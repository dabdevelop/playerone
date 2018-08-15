//
// Created by Orange on 2018/8/8.
//

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosio.token/eosio.token.hpp>
#include <math.h>

#define GAME_SYMBOL S(4, CGT)
#define TOKEN_CONTRACT N(eosio.token)
#define GAME_TOKEN_CONTRACT N(playeroneiss)
#define BURN_ACCOUNT N(blackholeeos)
#define FEE_ACCOUNT N(playeronefee)

typedef double real_type;

using namespace eosio;
using namespace std;

class playerone: public contract {
public:
    const int64_t _B = 5ll;
    const int64_t _A = 100ll - _B * 2;
    const int64_t _L = 1000000ll;
    const int64_t _D = _L / 4;
    const int64_t _INITIAL_PRICE = 100ll;
    const int64_t _MAX_SUPPLY_TIMES = 10ll;
    //TODO set the time to future game init time
    const int64_t _GAME_INIT_TIME = 0ll;
    //TODO 1 second to cool down
    const int64_t _ACTION_COOL_DOWN = 0ll;
    const int64_t _REFER_PRICE = 10000ll;

    playerone(account_name self)
        : contract(self), 
        _game(_self, _self),
        users(_self, _self),
        refers(_self, _self)
    {
        // Create a new game if not exists
        auto game_itr = _game.begin();
        if (game_itr == _game.end())
        {
            game_itr = _game.emplace(_self, [&](auto &g){
                g.gameid = _self;
                g.max_supply = asset(_L * _MAX_SUPPLY_TIMES * 10000ll, GAME_SYMBOL);
                g.start_time = _GAME_INIT_TIME;
            });
        }

        auto user_itr = users.find(FEE_ACCOUNT);
        if(user_itr == users.end()){
            user_itr = users.emplace(_self, [&](auto &u){
                u.name = FEE_ACCOUNT;
                u.parent = FEE_ACCOUNT;
                u.refer = 3;
                u.discount = 1;
            });
        }
        auto refer_itr = refers.find(FEE_ACCOUNT);
        if(refer_itr == refers.end() && user_itr->refer > 0){
            refer_itr = refers.emplace(_self, [&](auto &r){
                r.name = FEE_ACCOUNT;
            });
        }
    };

    /// @abi action 
    void transfer(account_name from, account_name to, asset quantity, string memo){
        require_auth(from);
        if (from == _self || to != _self) {
            return;
        }
        eosio_assert(quantity.is_valid(), "invalid token transfer");
        eosio_assert(quantity.amount > 0, "quantity must be positive");

        if(quantity.symbol == CORE_SYMBOL){
            eosio_assert(quantity.symbol == CORE_SYMBOL, "unexpected asset symbol input");
            if(memo == "deposit"){
                deposit(from, quantity, memo);
            } else {
                eosio_assert( now() >= _GAME_INIT_TIME, "game will start at 15341787619");
                buy(from, quantity, memo);
            }
        } else if(quantity.symbol == GAME_SYMBOL) {
            eosio_assert(quantity.symbol == GAME_SYMBOL, "unexpected asset symbol input");
            if(memo == "burn"){
                burn(from, quantity, memo);
            } else {
                sell(from, quantity, memo);
            }
        } else {
            eosio_assert(false, "do not send other funds to this contract");
        }
    };

    void buy(account_name account, asset quantity, string memo){
        eosio_assert(quantity.amount >= 10000ll && quantity.amount <= 100 * 10000ll, "quantity must in range 1 - 100 EOS");

        asset exchange_unit = asset(10 * 10000ll, CORE_SYMBOL);
        int64_t times = (quantity / exchange_unit) + 1;
        asset deposited_eos = asset(0, CORE_SYMBOL);
        asset insured_eos = asset(0, CORE_SYMBOL);
        asset exchanged_eos = asset(0, CORE_SYMBOL);
        asset issued_eos = asset(0, CORE_SYMBOL);
        

        auto user_itr = users.find(account);
        if(user_itr == users.end()){
            new_user(account, memo);
            user_itr = users.find(account);
        } else {
            eosio_assert(now() - user_itr->last_action >= _ACTION_COOL_DOWN, "action needs 1 second to cool down");
            users.modify(user_itr, 0, [&](auto &u) {
                u.last_action = now();
            });
        }

        asset fee = quantity;
        fee.amount = (fee.amount + 99) / 100; /// 1% fee (first round up)
        
        asset quant_after_fee = quantity;
        quant_after_fee -= fee;

        send_fee(account, fee);

        fee.amount = quant_after_fee.amount;
        if(user_itr->discount == 0){
            fee.amount = (fee.amount + 49) / 50; /// 2% fee (second round up)
        } else {
            fee.amount = (fee.amount + 99) / 100; /// 1% fee (discount half of the fee with a refer)
        }
        
        asset action_total_fee = fee;
        quant_after_fee -= fee;

        asset remain_eos = quant_after_fee;
        asset transfer_token = asset(0, GAME_SYMBOL);
        asset issue_token = asset(0, GAME_SYMBOL);
        auto game_itr = _game.begin();
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
            if(circulation > asset(10000 * 10000ll, GAME_SYMBOL) && token_balance > asset(0, GAME_SYMBOL)){
                crr = _crr(circulation);

                //TODO test the cast from uint64_t to real_type

                token_price = real_type(reserve_balance.amount) / (real_type(circulation.amount) * crr);

                //TODO test the cast from uint64_t to real_type and real_type to uint64_t

                asset token_per_exchange = asset(real_type(exchange_unit.amount) / token_price, GAME_SYMBOL);
                crr = _crr(circulation + token_per_exchange);

                //TODO test the cast from uint64_t to real_type

                token_price = real_type((reserve_balance + exchange_unit).amount) / (real_type(circulation.amount) * crr);
                token_per_exchange = asset(real_type(exchange_unit.amount) / token_price, GAME_SYMBOL);
                if(token_balance >= token_per_exchange){
                    circulation += token_per_exchange;
                    token_balance -= token_per_exchange;
                    transfer_token += token_per_exchange;
                    deposited_eos += exchange_unit;
                    remain_eos -= exchange_unit;
                    exchanged_eos += exchange_unit;
                    reserve_balance += exchange_unit;
                } else {
                    token_per_exchange = token_balance;
                    crr = _crr(circulation + token_per_exchange);

                    //TODO test the cast from uint64_t to real_type

                    token_price = real_type((reserve_balance + exchange_unit).amount) / (real_type(circulation.amount) * crr);
                    circulation += token_per_exchange;
                    token_balance -= token_per_exchange;
                    transfer_token += token_per_exchange;

                    //TODO test the cast from uint64_t to real_type and real_type to uint64_t

                    asset to_deposit_eos = asset(token_price * real_type(token_per_exchange.amount), CORE_SYMBOL);
                    deposited_eos += to_deposit_eos;
                    remain_eos -= to_deposit_eos;
                    exchanged_eos += to_deposit_eos;
                    reserve_balance += to_deposit_eos;
                }
                eosio_assert(token_price >= real_type(0.0), "invalid token price");
            } else {
                crr = _crr(circulation);

                //TODO test the cast from uint64_t to real_type and real_type to uint64_t

                asset to_issue_eos = asset( real_type(exchange_unit.amount) * crr, exchange_unit.symbol);
                real_type INITIAL_PRICE(_INITIAL_PRICE);
                real_type UNIT(10000.0);
                INITIAL_PRICE = INITIAL_PRICE / UNIT;

                //TODO test the cast from uint64_t to real_type and real_type to uint64_t

                asset token_per_issue = asset(real_type(to_issue_eos.amount) / INITIAL_PRICE, GAME_SYMBOL);
                circulation += token_per_issue;
                issue_token += token_per_issue;
                deposited_eos += to_issue_eos;
                insured_eos += exchange_unit - to_issue_eos;
                remain_eos -= exchange_unit;
                issued_eos += exchange_unit;
                reserve_balance += to_issue_eos;
            }
        }

        asset refund_eos = quant_after_fee - deposited_eos - insured_eos;

        eosio_assert(transfer_token <= game_itr->balance, "insufficient token balance");
        eosio_assert(refund_eos == remain_eos && refund_eos >= asset(0, CORE_SYMBOL) && refund_eos <= quant_after_fee, "invalid eos refund");
        eosio_assert(deposited_eos >= asset(0, CORE_SYMBOL) && insured_eos >= asset(0, CORE_SYMBOL), "eos deposit or insure must be positive");
        eosio_assert(transfer_token >= asset(0, GAME_SYMBOL) && issue_token >= asset(0, GAME_SYMBOL), "transfer and issue token should not be negetive");
        eosio_assert(exchanged_eos + issued_eos == deposited_eos + insured_eos && quant_after_fee - remain_eos == deposited_eos + insured_eos, "eos not equal");
        eosio_assert(transfer_token + issue_token >= asset(10000ll, GAME_SYMBOL) && transfer_token + issue_token <= asset(10000 * 10000ll, GAME_SYMBOL), "transfer and issue token must in range 1 - 10000");

        _game.modify(game_itr, 0, [&](auto &g) {
            g.reserve += deposited_eos;
            g.insure += insured_eos + action_total_fee;
            g.supply += issue_token;
            g.balance = token_balance;
            g.circulation = circulation;
        });

        if(refund_eos > asset(0, CORE_SYMBOL)){
            action(
                permission_level{_self, N(active)},
                TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, account, refund_eos, string("refund")))
            .send();
        }

        if(transfer_token > asset(0, GAME_SYMBOL)){
            action(
                permission_level{_self, N(active)},
                GAME_TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, account, transfer_token, string("buy")))
            .send();
        }

        if(issue_token > asset(0, GAME_SYMBOL)){
            action(
                permission_level{_self, N(active)},
                GAME_TOKEN_CONTRACT, N(issue),
                make_tuple(account, issue_token, string("issue")))
            .send();
        }

        //TODO check leaks here

        // asset real_eos_balance = eosio::token(TOKEN_CONTRACT).get_balance(_self, symbol_type(CORE_SYMBOL).name());
        // asset real_token_supply = eosio::token(GAME_TOKEN_CONTRACT).get_supply(symbol_type(GAME_SYMBOL).name());
        // eosio_assert(real_eos_balance == game_itr->reserve + game_itr->insure, "eos balance leaks");
        // eosio_assert(real_token_supply == game_itr->supply, "token supply leaks");
    }

    void sell(account_name account, asset quantity, string memo){
        eosio_assert(quantity.amount >= 10000ll && quantity.amount <= 5000 * 10000ll, "quantity must in range 1 - 5000 CGT");
        asset exchange_unit = asset(1000 * 10000ll, GAME_SYMBOL);
        asset remain_asset = quantity;
        int64_t times = (quantity / exchange_unit) + 1;
        asset transfer_eos = asset(0, CORE_SYMBOL);
        auto game_itr = _game.begin();
        asset reserve_balance = game_itr->reserve;
        asset token_supply = game_itr->supply;
        asset token_balance = game_itr->balance;
        eosio_assert(token_supply >= token_balance, "shit happens");
        asset circulation = token_supply - token_balance;
        real_type crr;
        real_type token_price;

        for(int64_t i = 0; i < times; i++){
            if(remain_asset <= asset(0, GAME_SYMBOL)){
                break;
            }
            if(exchange_unit > remain_asset){
                exchange_unit = remain_asset;
            }
            crr = _crr(circulation);

            //TODO test the cast from uint64_t to real_type

            token_price = real_type(reserve_balance.amount) / (real_type(circulation.amount) * crr);

            //TODO test the cast from uint64_t to real_type and real_type to uint64_t

            asset eos_per_exchange = asset(real_type(exchange_unit.amount) * token_price, CORE_SYMBOL);
            reserve_balance -= eos_per_exchange;
            if(reserve_balance < asset(0, CORE_SYMBOL)){
                eosio_assert(false, "insufficient reserve eos");
            }
            token_price = reserve_balance.amount / (circulation.amount * crr);

            //TODO test the cast from real_type to uint64_t and uint64_t to real_type

            eos_per_exchange = asset(real_type(exchange_unit.amount) * token_price, CORE_SYMBOL);
            transfer_eos += eos_per_exchange;
            circulation -= exchange_unit;
            remain_asset -= exchange_unit;
            token_balance += exchange_unit;

            eosio_assert(token_price >= real_type(0.0), "invalid token price");
        }

        eosio_assert(transfer_eos <= asset(100 * 10000ll, CORE_SYMBOL) && transfer_eos >= asset(10000ll, CORE_SYMBOL), "sell in range 1 - 100 eos");
        eosio_assert(remain_asset >= asset(0, GAME_SYMBOL) && quantity >= remain_asset, "remain asset is invalid");
        eosio_assert(quantity - remain_asset == token_balance - game_itr->balance, "exchange asset is not equal");
        eosio_assert(game_itr->reserve >= transfer_eos, "insufficient reserve eos");

        auto user_itr = users.find(account);
        if(user_itr == users.end()){
            new_user(account, memo);
            user_itr = users.find(account);
        } else {
            eosio_assert(now() - user_itr->last_action >= _ACTION_COOL_DOWN, "action needs 5 seconds to cool down");
            users.modify(user_itr, 0, [&](auto &u) {
                u.last_action = now();
            });
        }

        asset fee = transfer_eos;
        fee.amount = (fee.amount + 99) / 100; /// 1% fee (first round up)
        asset quant_after_fee = transfer_eos;
        quant_after_fee -= fee;

        send_fee(account, fee);

        fee.amount = quant_after_fee.amount;
        if(user_itr->discount == 0){
            fee.amount = (fee.amount + 49) / 50; /// 2% fee (second round up)
        } else {
            fee.amount = (fee.amount + 99) / 100; /// 1% fee (discount half of the fee with a refer)
        }

        asset action_total_fee = fee;
        quant_after_fee -= fee;

        _game.modify(game_itr, 0, [&](auto &g) {
            g.reserve -= quant_after_fee;
            g.insure += action_total_fee;
            g.balance = token_balance;
            g.circulation = circulation;
        });

        if(remain_asset > asset(0, GAME_SYMBOL)){
            action(
                permission_level{_self, N(active)},
                GAME_TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, account, remain_asset, string("refund")))
            .send();
        }

        if(quant_after_fee > asset(0, CORE_SYMBOL)){
            action(
                permission_level{_self, N(active)},
                TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, account, quant_after_fee, string("sell")))
            .send();
        }

        //TODO check leaks here

        // asset real_eos_balance = eosio::token(TOKEN_CONTRACT).get_balance(_self, symbol_type(CORE_SYMBOL).name());
        // asset real_token_supply = eosio::token(GAME_TOKEN_CONTRACT).get_supply(symbol_type(GAME_SYMBOL).name());
        // asset real_token_balance = eosio::token(GAME_TOKEN_CONTRACT).get_balance(_self, symbol_type(GAME_SYMBOL).name());
        // eosio_assert(real_eos_balance == game_itr->reserve + game_itr->insure, "eos balance leaks");
        // eosio_assert(real_token_supply == game_itr->supply, "token supply leaks");
        // eosio_assert(real_token_balance == game_itr->balance, "token balance leaks");
        // eosio_assert(real_token_supply - real_token_balance == game_itr->circulation && game_itr->circulation >= asset(0, GAME_SYMBOL), "circulation leaks");
    }

    void burn(account_name account, asset quantity, string memo){
        eosio_assert(quantity.amount >= 10000ll && quantity.amount <= 10000 * 10000ll, "quantity must in range 1 - 10000 CGT");
        
        auto game_itr = _game.begin();
        asset insure_balance = game_itr->insure;
        asset token_supply = game_itr->supply;
        asset token_balance = game_itr->balance;
        eosio_assert(token_supply >= token_balance, "shit happens");
        asset circulation = token_supply - token_balance;

        //TODO test the cast from uint64_t to real_type

        real_type token_price = real_type(insure_balance.amount) / real_type(circulation.amount);

        //TODO test the cast from uint64_t to real_type and real_type to uint64_t

        asset transfer_eos = asset(token_price * real_type(quantity.amount), CORE_SYMBOL);

        eosio_assert(transfer_eos <= asset(100 * 10000ll, CORE_SYMBOL) && transfer_eos >= asset(10000ll, CORE_SYMBOL), "burn in range 1 - 100 eos");
        eosio_assert(insure_balance >= transfer_eos, "insufficient insure eos");

        auto user_itr = users.find(account);
        if(user_itr == users.end()){
            new_user(account, memo);
            user_itr = users.find(account);
        } else {
            eosio_assert(now() - user_itr->last_action >= _ACTION_COOL_DOWN, "action needs 5 seconds to cool down");
            users.modify(user_itr, 0, [&](auto &u) {
                u.last_action = now();
            });
        }

        asset fee = transfer_eos;
        fee.amount = (fee.amount + 99) / 100; /// 1% fee (round up)
        asset action_total_fee = fee;
        asset quant_after_fee = transfer_eos;
        quant_after_fee -= fee;

        send_fee(account, fee);

        _game.modify(game_itr, 0, [&](auto &g) {
            g.insure -= quant_after_fee;
            g.supply -= quantity;
            g.circulation -= quantity;
            g.burn += quantity;
        });

        if(quantity > asset(0, GAME_SYMBOL)){
            action(
                permission_level{_self, N(active)},
                GAME_TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, BURN_ACCOUNT, quantity, string("burn token")))
            .send();
        }

        if(quant_after_fee > asset(0, CORE_SYMBOL)){
            action(
                permission_level{_self, N(active)},
                TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, account, quant_after_fee, string("burn return")))
            .send();
        }

        //TODO check leaks here

        // asset real_eos_balance = eosio::token(TOKEN_CONTRACT).get_balance(_self, symbol_type(CORE_SYMBOL).name());
        // asset real_token_supply = eosio::token(GAME_TOKEN_CONTRACT).get_supply(symbol_type(GAME_SYMBOL).name());
        // eosio_assert(real_eos_balance == game_itr->reserve + game_itr->insure, "eos balance leaks");
        // eosio_assert(real_token_supply == game_itr->supply, "token supply leaks");
    }

    void deposit(account_name account, asset quantity, string memo){
        auto game_itr = _game.begin();
        auto user_itr = users.find(account);
        if(quantity.amount >= _REFER_PRICE / 2){
            uint32_t refer = 0;
            if(quantity.amount >= _REFER_PRICE){
                refer = quantity.amount / _REFER_PRICE;
            }
            if(user_itr == users.end()){
                new_user(account, memo);
                user_itr = users.find(account);
            }
            if(user_itr != users.end()){
                users.modify(user_itr, 0, [&](auto &u) {
                    u.refer += refer;
                    u.last_action = now();
                });
            }
            auto refer_itr = refers.find(account);
            if(refer_itr == refers.end() && user_itr->refer > 0){
                refer_itr = refers.emplace(account, [&](auto &r){
                    r.name = account;
                });
                _game.modify(game_itr, 0, [&](auto& g){
                    g.next_refer = account;
                });
            }
        }
        
        _game.modify(game_itr, 0, [&](auto &g) {
            g.insure += quantity;
        });

        //TODO check leaks here

        // auto eos_token = eosio::token(TOKEN_CONTRACT);
        // asset real_eos_balance = eos_token.get_balance(_self, symbol_type(CORE_SYMBOL).name());
        // eosio_assert(real_eos_balance == game_itr->reserve + game_itr->insure, "eos balance leaks");
    }

    void new_user(account_name account, string memo){
        auto user_itr = users.find(account);
        if(user_itr != users.end()) return;

        uint32_t discount = 0;
        auto parent = string_to_name(memo.c_str());
        auto parent_itr = users.find(parent);
        auto game_itr = _game.begin();
        if(parent_itr == users.end() || parent_itr->refer <= 0 || account == parent){
            parent = FEE_ACCOUNT;
            parent_itr = users.find(FEE_ACCOUNT);
            if(refers.begin() != refers.end() && game_itr->next_refer != BURN_ACCOUNT){
                auto refer_itr = refers.find(game_itr->next_refer);
                if(refer_itr != refers.end()){
                    auto refer_user_itr = users.find(refer_itr->name);
                    if(refer_user_itr != users.end()){
                        parent = refer_user_itr->name;
                        parent_itr = users.find(refer_user_itr->name);
                        const auto& refer_obj = *refer_itr;
                        ++refer_itr;
                        if(refer_user_itr->refer <= 1 && refer_user_itr->name != FEE_ACCOUNT){
                            refers.erase(refer_obj);
                        }
                        if(refer_itr == refers.end()){
                            if(refers.begin() == refers.end()){
                                _game.modify(game_itr, 0, [&](auto& g){
                                    g.next_refer = BURN_ACCOUNT;
                                });
                            } else {
                                _game.modify(game_itr, 0, [&](auto& g){
                                    g.next_refer = refers.begin()->name;
                                });
                            }
                        } else {
                            _game.modify(game_itr, 0, [&](auto& g){
                                g.next_refer = refer_itr->name;
                            });
                        }
                    }
                }
            }
        }

        if(parent_itr->refer > 0){
            discount = 1;
            users.modify(parent_itr, 0, [&](auto &u){
                u.refer --;
            });
        }
        
        users.emplace(account, [&](auto &u) {
            u.name = account;
            u.parent = parent;
            u.discount = discount;
            u.last_action = now();
        });
    }

    void send_fee(account_name account, asset fee){
        if (fee.amount > 0){
            auto refer_fee = fee;
            refer_fee = fee / 2;
            fee -= refer_fee;

            auto user_itr = users.find(account);
            if(user_itr == users.end()) return;
            auto parent_itr = users.find(user_itr->parent);
            if(parent_itr == users.end()) return;

            action(
                permission_level{_self, N(active)},
                TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, parent_itr->name, fee, string("refer fee")))
            .send();

            if (refer_fee.amount > 0)
            {
                parent_itr = users.find(parent_itr->parent);
                if(parent_itr == users.end()) return;
                action(
                    permission_level{_self, N(active)},
                    TOKEN_CONTRACT, N(transfer),
                    make_tuple(_self, parent_itr->name, refer_fee, string("refer fee")))
                .send();
            }
        }
    }

    private:
    real_type _crr(asset circulation) {
        eosio_assert(circulation.amount >= 0, "shit happens");
        real_type A(_A);
        real_type B(_B);
        real_type L(_L);
        real_type D(_D);
        real_type ONE(1.0);
        real_type H(100.0);
        real_type UNIT(10000.0);
        real_type E(2.71828182845904);

        real_type X(circulation.amount);
        X = X / UNIT;
        
        real_type R = ONE / (ONE + pow(E, (X - L) / D)) * A + B;
        eosio_assert(R >= B && R <= B + A, "shit happens");
        return R / H;
    }

    // @abi table game i64
    struct game{
        account_name gameid;
        asset reserve = asset(0, CORE_SYMBOL);
        asset insure = asset(0, CORE_SYMBOL);
        asset max_supply = asset(0, CORE_SYMBOL);
        asset supply = asset(0, GAME_SYMBOL);
        asset balance = asset(0, GAME_SYMBOL);
        asset circulation = asset(0, GAME_SYMBOL);
        asset burn = asset(0, GAME_SYMBOL);
        uint64_t start_time;
        account_name next_refer;

        uint64_t primary_key() const { return gameid; }
        EOSLIB_SERIALIZE(game, (gameid)(reserve)(insure)(max_supply)(supply)(balance)(circulation)(burn)(start_time)(next_refer))
    };
    typedef eosio::multi_index<N(game), game> game_index;
    game_index _game;

    // @abi table users i64
    struct user{
        account_name name;
        account_name parent;
        uint64_t last_action;
        uint32_t refer = 0;
        uint32_t discount = 0;

        uint64_t primary_key() const { return name; }
        EOSLIB_SERIALIZE(user, (name)(parent)(last_action)(refer)(discount))
    };
    typedef eosio::multi_index<N(users), user> user_index;
    user_index users;

    // @abi table refers i64
    struct refer{
        account_name name;
        uint64_t primary_key() const { return name; }
        EOSLIB_SERIALIZE(refer, (name))
    };
    typedef eosio::multi_index<N(refers), refer> refer_index;
    refer_index refers;
};

#define EOSIO_ABI_EX( TYPE, MEMBERS ) \
extern "C" { \
    void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
        auto self = receiver; \
        if( action == N(onerror)) { \
            /* onerror is only valid if it is for the "eosio" code account and authorized by "eosio"'s "active permission */ \
            eosio_assert(code == N(eosio), "onerror action's are only valid from the \"eosio\" system account"); \
        } \
        if((code == TOKEN_CONTRACT && action == N(transfer)) || (code == GAME_TOKEN_CONTRACT && action == N(transfer))) { \
            TYPE thiscontract( self ); \
            switch( action ) { \
                EOSIO_API( TYPE, MEMBERS ) \
            } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
        } \
    } \
} \

EOSIO_ABI_EX(playerone, (transfer))