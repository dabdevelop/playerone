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

typedef double real_type;

using namespace eosio;
using namespace std;

class playerone: public contract {
public:
    const real_type _A = real_type(0.9);
    const real_type _B = real_type(0.1);
    const real_type _L = real_type(2000000);
    const real_type _D = real_type(500000);
    const real_type _INITIAL_PRICE = real_type(0.01);

    playerone(account_name self)
        : contract(self), 
          _games(self, self)
    {
        // Create a new game if not exists
        auto game_itr = _games.find(self);
        if (game_itr == _games.end())
        {
            game_itr = _games.emplace(_self, [&](auto &new_game){
                new_game.gameid = self;
            });
        }
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
            eosio_assert(false, "shit happens");
        }
    };

    void buy(account_name account, asset quantity){
        require_auth(account);
        eosio_assert(quantity.symbol == CORE_SYMBOL, "unexpected asset symbol input");
        eosio_assert(quantity.amount >= 0.01 && quantity.amount <= 100, "quantity must in range 0.01 - 100");

        auto eos_token = eosio::token(N(eosio.token));
        auto balance = eos_token.get_supply(symbol_type(CORE_SYMBOL).name());
        auto token_supply = eos_token.get_supply(symbol_type(GAME_SYMBOL).name());
        auto token_balance = eos_token.get_balance(_self, symbol_type(GAME_SYMBOL).name());
        eosio_assert(token_supply >= token_balance, "shit happens");
        auto circulation = token_supply - token_balance;
        real_type _crr = crr(circulation);
        auto game_itr = _games.find(_self);
        
        _games.modify(game_itr, 0, [&](auto &game) {
            game.balance = balance;
            game.supply = token_supply;
            game.token_balance = token_balance;
            game.counter++;
            game.crr = _crr;
        });

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
    real_type crr(asset circulation) {
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
        asset balance = asset(0, CORE_SYMBOL);
        asset max_supply = asset(10000000, GAME_SYMBOL);
        asset supply = asset(0, GAME_SYMBOL);
        asset token_balance = asset(0, GAME_SYMBOL);
        asset circulation = asset(0, GAME_SYMBOL);
        asset burn = asset(0, GAME_SYMBOL);
        real_type crr = real_type(0.0);
        uint64_t counter = 0;
        uint64_t start_time = current_time();

        uint64_t primary_key() const { return gameid; }
        EOSLIB_SERIALIZE(game, (gameid)(balance)(token_balance)(max_supply)(supply)(circulation)(burn)(counter)(start_time))
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
        if((code == N(eosio.token) && action == N(transfer))) { \
            TYPE thiscontract( self ); \
            switch( action ) { \
                EOSIO_API( TYPE, MEMBERS ) \
            } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
        } \
    } \
} \

EOSIO_ABI_EX(playerone, (transfer))