//
// Created by Orange on 2018/8/8.
//

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/crypto.h>
#include <eosiolib/currency.hpp>
#include <eosio.token/eosio.token.hpp>
#include <math.h>
#include <cstring>

#define GAME_SYMBOL S(4, CGT)
#define TOKEN_CONTRACT N(eosio.token)

typedef double real_type;

using namespace eosio;
using namespace std;

class playerone: public contract {
public:
    const real_type _A = real_type(0.9);
    const real_type _B = real_type(0.1);
    const real_type _L = real_type(200000);
    const real_type _D = real_type(50000);
    const real_type _INITIAL_PRICE = real_type(0.01);
    const real_type _MAX_SUPPLY_TIMES = 20;

    playerone(account_name self)
        : contract(self), 
          _games(_self, _self)
    {
        // Create a new game if not exists
        auto game_itr = _games.find(_self);
        if (game_itr == _games.end())
        {
            game_itr = _games.emplace(_self, [&](auto &new_game){
                new_game.gameid = _self;
                new_game.max_supply = asset(_L * _MAX_SUPPLY_TIMES, GAME_SYMBOL);
            });
        }

        action(
                permission_level{_self, N(active)},
                TOKEN_CONTRACT, N(create),
                make_tuple(_self, game_itr->max_supply))
        .send();

    };

    void transfer(account_name from, account_name to, asset quantity, string memo){
        if (from == _self || to != _self) {
            return;
        }
        eosio_assert(quantity.is_valid(), "invalid token transfer");
        eosio_assert(quantity.amount > 0, "quantity must be positive");

        if(quantity.symbol == CORE_SYMBOL){
            eosio_assert(quantity.symbol == CORE_SYMBOL, "unexpected asset symbol input");
            if(memo == "deposit"){
                deposit(quantity);
            } else {
                buy(from, quantity);
            }
        } else if(quantity.symbol == GAME_SYMBOL) {
            eosio_assert(quantity.symbol == GAME_SYMBOL, "unexpected asset symbol input");
            if(memo == "burn"){
                burn(from, quantity);
            } else {
                sell(from, quantity);
            }
        } else {
            eosio_assert(false, "invalid token transfer");
        }
    };

    void buy(account_name account, asset quantity){
        require_auth(account);
        eosio_assert(quantity.symbol == CORE_SYMBOL, "unexpected asset symbol input");
        eosio_assert(quantity.amount >= 0.01 && quantity.amount <= 100, "quantity must in range 0.01 - 100");

        asset exchange_unit = asset(1, CORE_SYMBOL);
        int64_t times = (quantity / exchange_unit) + 1;

        asset deposited_eos = asset(0, CORE_SYMBOL);
        asset insured_eos = asset(0, CORE_SYMBOL);
        asset exchanged_eos = asset(0, CORE_SYMBOL);
        asset issued_eos = asset(0, CORE_SYMBOL);
        asset remain_eos = quantity;
        asset transfer_token_amount = asset(0, GAME_SYMBOL);
        asset issue_token_amount = asset(0, GAME_SYMBOL);

        game_index _games(_self, _self);
        auto game_itr = _games.find(_self);

        eosio_assert(game_itr != _games.end(), "no game settled");

        asset reserve_balance = game_itr->reserve;
        asset token_supply = game_itr->supply;
        asset token_balance = game_itr->balance;

        eosio_assert(token_supply >= token_balance, "shit happens");

        asset circulation = token_supply - token_balance;
        real_type crr;
        real_type token_price;
        for(int64_t i = 0; i < times; i++){
            if(remain_eos <= asset(0, CORE_SYMBOL)){
                break;
            }
            if(exchange_unit > remain_eos){
                exchange_unit = remain_eos;
            }
            if(circulation > asset(10000, CORE_SYMBOL) && token_balance > asset(0, CORE_SYMBOL)){
                crr = _crr(circulation);
                token_price = reserve_balance / (circulation * crr);
                asset token_per_exchange = exchange_unit / token_price;
                crr = _crr(circulation + token_per_exchange);
                token_price = (reserve_balance + exchange_unit) / (circulation * crr);
                token_per_exchange = exchange_unit / token_price;
                if(token_balance >= token_per_exchange){
                    circulation += token_per_exchange;
                    token_balance -= token_per_exchange;
                    // TODO unit not match
                    transfer_token_amount += token_per_exchange;
                    deposited_eos += exchange_unit;
                    remain_eos -= exchange_unit;
                    exchanged_eos += exchange_unit;
                    reserve_balance += exchange_unit;
                } else {
                    token_per_exchange = token_balance;
                    crr = _crr(circulation + token_per_exchange);
                    token_price = (reserve_balance + exchange_unit) / (circulation * crr);
                    circulation += token_per_exchange;
                    token_balance += token_per_exchange;
                    transfer_token_amount += token_per_exchange;
                    asset to_deposit_eos = token_price * token_per_exchange;
                    deposited_eos += to_deposit_eos;
                    remain_eos -= to_deposit_eos;
                    exchanged_eos += to_deposit_eos;
                    reserve_balance += to_deposit_eos;
                }
            } else {
                crr = _crr(circulation);
                asset to_issue_eos = exchange_unit * crr;
                asset token_per_issue = to_issue_eos / _INITIAL_PRICE;
                circulation += token_per_issue;
                issue_token_amount += token_per_issue;
                deposited_eos += to_issue_eos;
                insured_eos += exchange_unit - to_issue_eos;
                remain_eos -= exchange_unit;
                issued_eos += exchange_unit;
                reserve_balance += to_issue_eos;
            }
        }

        asset refund_eos = quantity - deposited_eos - insured_eos;
        eosio_assert(refund_eos >= asset(0, CORE_SYMBOL) && refund_eos <= quantity, "invalid refund amount");
        eosio_assert(exchanged_eos + issued_eos == deposited_eos + insured_eos, "exchanged value not equal");
        eosio_assert(refund_eos == remain_eos, "refund value not equal");
        eosio_assert(quantity - remain_eos == deposited_eos + insured_eos, "refund value not equal");

        _games.modify(game_itr, 0, [&](auto &game) {
            game.reserve += deposited_eos;
            game.insure += insured_eos;
            game.supply += issue_token_amount;
            game.balance = token_balance;
            game.circulation = circulation;
            game.counter++;
            game.crr = crr;
        });

        if(refund_eos > asset(0, CORE_SYMBOL)){
            action(
                permission_level{_self, N(active)},
                TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, account, refund_eos, string("refund")))
            .send();
        }

        if(transfer_token_amount > asset(0, GAME_SYMBOL)){
            action(
                permission_level{_self, N(active)},
                TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, account, transfer_token_amount, string("transfer")))
            .send();
        }

        if(issue_token_amount > asset(0, GAME_SYMBOL)){
            action(
                permission_level{_self, N(active)},
                TOKEN_CONTRACT, N(issue),
                make_tuple(account, issue_token_amount, string("issue")))
            .send();
        }

        auto eos_token = eosio::token(TOKEN_CONTRACT);
        asset real_eos_balance = eos_token.get_balance(_self, symbol_type(CORE_SYMBOL).name());
        asset real_token_supply = eos_token.get_supply(symbol_type(GAME_SYMBOL).name());
        asset real_token_balance = eos_token.get_balance(_self, symbol_type(GAME_SYMBOL).name());
        eosio_assert(real_eos_balance == game_itr->reserve + game_itr->insure, "eos balance leaks");
        eosio_assert(real_token_supply == game_itr->supply, "token supply leaks");
        eosio_assert(real_token_balance == game_itr->balance, "token balance leaks");
        eosio_assert(real_token_supply - real_token_balance == game_itr->circulation && game_itr->circulation >= asset(0, GAME_SYMBOL), "circulation leaks");

    }

    void sell(account_name account, asset quantity){
        require_auth(account);
        eosio_assert(quantity.symbol == GAME_SYMBOL, "unexpected asset symbol input");
        eosio_assert(quantity.amount >= 1 && quantity.amount <= 10000, "quantity must in range 1 - 10000");

    }

    void burn(account_name account, asset quantity){
        require_auth(account);
        eosio_assert(quantity.symbol == GAME_SYMBOL, "unexpected asset symbol input");
        eosio_assert(quantity.amount >= 1 && quantity.amount <= 10000, "quantity must in range 1 - 10000");

    }

    void deposit(asset quantity){

    }

    private:
    real_type _crr(asset circulation) {
        real_type _X(circulation.amount);
        real_type ONE(1.0);
        real_type E(2.71828182845904);
        real_type R = ONE / (ONE + pow(E, (_X - _L) / _D)) * _A + _B;
        eosio_assert(R >= _B && R <= _B + _A, "unexpected result");
        return R;
    }

    // @abi table game i64
    struct game{
        uint64_t gameid;
        asset reserve = asset(0, CORE_SYMBOL);
        asset insure = asset(0, CORE_SYMBOL);
        asset max_supply;
        asset supply = asset(0, GAME_SYMBOL);
        asset balance = asset(0, GAME_SYMBOL);
        asset circulation = asset(0, GAME_SYMBOL);
        asset burn = asset(0, GAME_SYMBOL);
        asset eos_in = asset(0, CORE_SYMBOL);
        asset eos_out = asset(0, CORE_SYMBOL);
        asset token_in = asset(0, GAME_SYMBOL);
        asset token_out = asset(0, GAME_SYMBOL);
        real_type crr = real_type(0.0);
        uint64_t counter = 0;
        uint64_t start_time = current_time();

        uint64_t primary_key() const { return gameid; }
        EOSLIB_SERIALIZE(game, (gameid)(reserve)(insure)(max_supply)(supply)(balance)(circulation)(burn)(eos_in)(eos_out)(token_in)(token_out)(crr)(counter)(start_time))
    };
    typedef eosio::multi_index<N(game), game> game_index;
    game_index _games;

};

#define EOSIO_ABI_EX( TYPE, MEMBERS ) \
extern "C" { \
    void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
        auto self = receiver; \
        if( action == N(onerror)) { \
            /* onerror is only valid if it is for the "eosio" code account and authorized by "eosio"'s "active permission */ \
            eosio_assert(code == N(eosio), "onerror action's are only valid from the \"eosio\" system account"); \
        } \
        if((code == TOKEN_CONTRACT && action == N(transfer))) { \
            TYPE thiscontract( self ); \
            switch( action ) { \
                EOSIO_API( TYPE, MEMBERS ) \
            } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
        } \
    } \
} \

EOSIO_ABI_EX(playerone, (transfer))