//
// Created by Orange on 2018/8/8.
//

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <math.h>

#define GAME_SYMBOL S(4, CGT)
#define TOKEN_CONTRACT N(eosio.token)
#define GAME_TOKEN_CONTRACT N(playeroneiss)
#define BURN_ACCOUNT N(blackholeeos)
#define FEE_ACCOUNT N(playeronefee)
#define CPUBANK_ACCOUNT N(eosiocpubank)

typedef double real_type;

using namespace eosio;
using namespace std;

class playerone: public contract {
public:
    const int64_t _B = 5ll;
    const int64_t _A = 100ll - _B * 2;
    const int64_t _L = 2500000ll;
    const int64_t _D = _L / 4;
    const int64_t _INITIAL_PRICE = 100ll;
    const int64_t _MAX_SUPPLY_TIMES = 10ll;
    const int64_t _GAME_INIT_TIME = 1535717288ll;  // 2018/8/31 20:8:8
    const int64_t _GAME_PRESALE_TIME = _GAME_INIT_TIME + 10 * 60ll;
    const int64_t _ACTION_COOL_DOWN = 1ll;
    const int64_t _REWARD_COOL_DOWN = 24 * 60 * 60ll;
    const int64_t _UNIT = 10000ll;
    const int64_t _MAX_IN_PRESALE = 10 * _UNIT;
    const int64_t _REFER_PRICE = _UNIT;

    playerone(account_name self)
        : contract(self), 
        _game(_self, _self),
        refers(_self, _self),
        invitations(_self, _self)
    {
        // Create a new game if not exists
        auto game_itr = _game.begin();
        if (game_itr == _game.end())
        {
            game_itr = _game.emplace(_self, [&](auto& g){
                g.gameid = _self;
                g.max_supply = asset(_L * _MAX_SUPPLY_TIMES * _UNIT, GAME_SYMBOL);
                g.start_time = _GAME_INIT_TIME;
                g.reward_time = _GAME_PRESALE_TIME + _REWARD_COOL_DOWN;
            });
        }
        user_table userinfo(_self, FEE_ACCOUNT);
        auto user_itr = userinfo.find(game_itr->gameid);
        if(user_itr == userinfo.end()){
            user_itr = userinfo.emplace(_self, [&](auto& u){
                u.gameid = game_itr->gameid;
                u.name = FEE_ACCOUNT;
                u.parent = FEE_ACCOUNT;
                u.refer = 100;
                u.discount = 1;
            });
        }

        auto refer_itr = refers.find(FEE_ACCOUNT);
        if(refer_itr == refers.end() && user_itr->refer > 0){
            refer_itr = refers.emplace(_self, [&](auto& r){
                r.name = FEE_ACCOUNT;
            });
        }
    };

    void eosio_token_transfer(account_name from, account_name to, asset quantity, string memo){
        require_auth(from);
        if (from == _self || to != _self) {
            return;
        }
        eosio_assert(quantity.is_valid(), "invalid token transfer");
        eosio_assert(quantity.symbol == CORE_SYMBOL, "unexpected asset symbol input");

        transfer(from, to, quantity, memo);
    };

    void game_token_transfer(account_name from, account_name to, asset quantity, string memo){
        require_auth(from);
        if (from == _self || to != _self) {
            return;
        }
        eosio_assert(quantity.is_valid(), "invalid token transfer");
        eosio_assert(quantity.symbol == GAME_SYMBOL, "unexpected asset symbol input");

        transfer(from, to, quantity, memo);
    };

    void transfer(account_name from, account_name to, asset quantity, string memo){
        require_auth(from);
        if (from == _self || to != _self) {
            return;
        }
        eosio_assert(quantity.is_valid(), "invalid token transfer");

        auto game_itr = _game.begin();

        if(quantity.symbol == CORE_SYMBOL){
            if(quantity.amount == 1ll){
                if(memo.size() <= 0 || memo.size() > 12){
                    // 用户通过向合约转账0.0001EOS取回推荐人奖金
                    claim_fee(from);
                } else {
                    // 预售前可以通过发送邀请获得免费的预售额度。发送邀请的方式为：向合约转账0.0001EOS并且备注未注册的EOS账号，将成为他的上级（需要消耗少量RAM），每个邀请增加1EOS预售额度(邀请码会减少一个)
                    string from_str = name_to_string(from);
                    account_name to_user = string_to_name(memo.c_str());
                    auto invitation_itr = invitations.find(to_user);
                    user_table to_userinfo(_self, to_user);
                    auto to_user_itr = to_userinfo.find(game_itr->gameid);
                    user_table from_userinfo(_self, from);
                    auto from_user_itr = from_userinfo.find(game_itr->gameid);
                    if(from_user_itr != from_userinfo.end() && invitation_itr == invitations.end() && to_user_itr == to_userinfo.end()){
                        action(
                            permission_level{_self, N(active)},
                            TOKEN_CONTRACT, N(transfer),
                            make_tuple(_self, to_user, quantity, from_str + "邀请您参与头号玩家，通过邀请码注册有机会减少一半的手续费。网址: http://eosplayer.one/#/?ref=" + from_str))
                        .send();
                        invitation_itr = invitations.emplace(_self, [&](auto& i){
                            i.to = to_user;
                            i.from = from;
                        });
                        if(from_user_itr->refer > 0){
                            from_userinfo.modify(from_user_itr, _self, [&](auto& u){
                                    u.refer --;
                            });
                            to_user_itr = to_userinfo.emplace(_self, [&](auto& u){
                                u.gameid = game_itr->gameid;
                                u.name = to_user;
                                u.parent = from;
                                u.discount = 1;
                            });
                            if(from_user_itr->quota < 200 && now() < _GAME_INIT_TIME ){
                                from_userinfo.modify(from_user_itr, _self, [&](auto& u){
                                    u.quota += 1;
                                });
                            }
                        }
                    }
                }
            } else if(quantity.amount == 2ll){
                // 头号通过向合约转账0.0002EOS获得头号奖金
                // 奖励冷却时间是24小时，每次获得头号奖金池的10%，单次奖金得超过10EOS
                // 如果头号位置更替，奖励冷却时间将重置
                claim_reward(from);
            } else if(quantity.amount == 3ll){
                // 头号通过向合约转账0.0003EOS解除抵押，将消耗抵押CGT的10%
                unstake(from);
            } else if(memo == "deposit"){
                // 通过向合约转账备注deposit存入EOS，用于购买邀请码，预售结束前获得等量预售额外额度，以及之后生态接入
                // 存入的EOS将分红给所有流通CGT
                // 购买邀请码将有机会获得新用户交易手续费的分红
                deposit(from, quantity);
            } else if(memo == "reward"){
                // 通过向合约转账备注reward存入头号奖励
                // 存入的EOS将鼓励玩家竞选头号
                deposit_reward(quantity);
            } else if(memo == "1d" || memo == "4d" || memo == "7d") {
                // 通过向合约转账0.01 - 1 EOS并且备注1d/4d/7d为自己租赁CPU
                eosio_assert(quantity.amount >= 100ll && quantity.amount <= 1 * _UNIT, "租用CPU的EOS区间是 0.01 - 1 EOS");
                action(
                    permission_level{_self, N(active)},
                    TOKEN_CONTRACT, N(transfer),
                    make_tuple(_self, CPUBANK_ACCOUNT, quantity, memo + " " + name_to_string(from)))
                .send();
            } else {
                eosio_assert( now() >= _GAME_INIT_TIME, "游戏开始时间是2018/8/31 20:8:8");
                if( now() < _GAME_PRESALE_TIME ){
                    user_table userinfo(_self, from);
                    auto user_itr = userinfo.find(game_itr->gameid);
                    // 预售结束前可以通过存入EOS获得等量的预售额度，存入的EOS对全部流通CGT分红，不可退回。相当于两倍的价格参与预售
                    // 预售中每次买入会有平均30秒的冷却时间，连续两次买入间隔越短，下一次买入冷却时间越长，t = 225 / (dt + 1)，t为冷却时间，dt是冷却后等待的时间。预售结束之后冷却时间降为1秒。
                    if(user_itr == userinfo.end() || quantity.amount > user_itr->quota * _UNIT + _MAX_IN_PRESALE || quantity.amount <= _MAX_IN_PRESALE ){
                        eosio_assert( quantity.amount >= _UNIT && quantity.amount <= _MAX_IN_PRESALE, "预售额外份额不足，请单次购买 1 - 10 EOS");
                    } else {
                        // 超出10EOS的部分将从预售额度里面扣除
                        asset quota = quantity;
                        quota -= asset(_MAX_IN_PRESALE, CORE_SYMBOL);
                        eosio_assert( user_itr->quota * _UNIT >= quota.amount, "预售额外份额不足，请单次购买 1 - 10 EOS");
                        userinfo.modify(user_itr, _self, [&](auto& u){
                            u.quota -= quota.amount / _UNIT;
                        });
                    }
                }
                buy(from, quantity, memo);
            }
        } else if(quantity.symbol == GAME_SYMBOL) {
            eosio_assert( now() >= _GAME_PRESALE_TIME, "预售阶段不能够销毁、抵押、出售CGT");
            if(memo == "burn"){
                burn(from, quantity);
            } else if(memo == "stake"){
                // 通过向合约转账足够的CGT并且备注stake竞争头号
                stake(from, quantity);
            } else {
                sell(from, quantity, memo);
            }
        } else {
            eosio_assert(false, "不要转入其他代币资产");
        }
    };

    void buy(account_name account, asset quantity, string memo){
        eosio_assert(quantity.amount >= _UNIT && quantity.amount <= 100 * _UNIT, "买入CGT区间为 1 - 100 EOS");

        asset exchange_unit = asset(20 * _UNIT, CORE_SYMBOL);
        int64_t times = (quantity / exchange_unit) + 1;
        asset deposited_eos = asset(0, CORE_SYMBOL);
        asset insured_eos = asset(0, CORE_SYMBOL);
        asset exchanged_eos = asset(0, CORE_SYMBOL);
        asset issued_eos = asset(0, CORE_SYMBOL);
        auto game_itr = _game.begin();
        user_table userinfo(_self, account);
        auto user_itr = userinfo.find(game_itr->gameid);
        if(user_itr == userinfo.end()){
            new_user(account, memo, _self);
            user_itr = userinfo.find(game_itr->gameid);
        } else {
            uint64_t now_action = now();
            eosio_assert( now_action >= user_itr->last_action + _ACTION_COOL_DOWN, "操作太频繁，需要时间冷却");
            if( now_action < _GAME_PRESALE_TIME){
                now_action += 225ll / (now_action - user_itr->last_action + 1);
            }
            userinfo.modify(user_itr, _self, [&](auto& u) {
                u.last_action = now_action;
            });
        }    

        asset fee = quantity;
        fee.amount = (fee.amount + 99) / 100; /// 1% fee (first round up)
        
        asset quant_after_fee = quantity;
        quant_after_fee -= fee;

        collect_fee(account, fee);

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
        asset reserve_balance = game_itr->reserve;
        asset token_supply = game_itr->supply;
        asset token_balance = game_itr->balance;
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
            if(circulation > asset(100000 * _UNIT, GAME_SYMBOL) && token_balance > asset(0, GAME_SYMBOL)){
                crr = _crr(circulation);
                token_price = real_type(reserve_balance.amount) / (real_type(circulation.amount) * crr);
                asset token_per_exchange = asset(real_type(exchange_unit.amount) / token_price, GAME_SYMBOL);
                crr = _crr(circulation + token_per_exchange);
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
                    token_price = real_type((reserve_balance + exchange_unit).amount) / (real_type(circulation.amount) * crr);
                    circulation += token_per_exchange;
                    token_balance -= token_per_exchange;
                    transfer_token += token_per_exchange;
                    asset to_deposit_eos = asset(token_price * real_type(token_per_exchange.amount), CORE_SYMBOL);
                    deposited_eos += to_deposit_eos;
                    remain_eos -= to_deposit_eos;
                    exchanged_eos += to_deposit_eos;
                    reserve_balance += to_deposit_eos;
                }
                eosio_assert(token_price >= real_type(0.0), "invalid token price");
            } else {
                crr = _crr(circulation);
                asset to_issue_eos = asset(real_type(exchange_unit.amount) * crr, exchange_unit.symbol);
                real_type INITIAL_PRICE(_INITIAL_PRICE);
                real_type UNIT(_UNIT);
                INITIAL_PRICE = INITIAL_PRICE / UNIT;
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
        eosio_assert(transfer_token + issue_token >= asset(_UNIT, GAME_SYMBOL) && transfer_token + issue_token <= asset(10000 * _UNIT, GAME_SYMBOL), "transfer and issue token must in range 1 - 10000");

        _game.modify(game_itr, _self, [&](auto& g) {
            g.reserve += deposited_eos;
            g.insure += insured_eos + fee / 2;
            g.reward += action_total_fee - fee / 2;
            g.supply += issue_token;
            g.balance = token_balance;
            g.circulation = circulation;
        });

        if(refund_eos > asset(0, CORE_SYMBOL)){
            _game.modify(game_itr, _self, [&](auto& g){
                g.fee += refund_eos;
            });
            userinfo.modify(user_itr, _self, [&](auto& u){
                u.reward += refund_eos;
            });
        }

        if(transfer_token > asset(0, GAME_SYMBOL)){
            action(
                permission_level{_self, N(active)},
                GAME_TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, account, transfer_token, string("购买CGT，感谢您支持头号玩家。 网址: http://eosplayer.one")))
            .send();
        }

        if(issue_token > asset(0, GAME_SYMBOL)){
            action(
                permission_level{_self, N(active)},
                GAME_TOKEN_CONTRACT, N(issue),
                make_tuple(account, issue_token, string("发行新CGT，感谢您支持头号玩家。 网址: http://eosplayer.one")))
            .send();
        }
    }

    void sell(account_name account, asset quantity, string memo){
        eosio_assert(quantity.amount >= _UNIT && quantity.amount <= 10000 * _UNIT, "卖出CGT区间为 1 - 10000 CGT");
        asset exchange_unit = asset(2000 * _UNIT, GAME_SYMBOL);
        asset remain_asset = quantity;
        int64_t times = (quantity / exchange_unit) + 1;
        asset transfer_eos = asset(0, CORE_SYMBOL);
        auto game_itr = _game.begin();
        asset reserve_balance = game_itr->reserve;
        asset token_supply = game_itr->supply;
        asset token_balance = game_itr->balance;
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
            token_price = real_type(reserve_balance.amount) / (real_type(circulation.amount) * crr);
            asset eos_per_exchange = asset(real_type(exchange_unit.amount) * token_price, CORE_SYMBOL);
            reserve_balance -= eos_per_exchange;
            if(reserve_balance < asset(0, CORE_SYMBOL)){
                eosio_assert(false, "insufficient reserve eos");
            }
            token_price = reserve_balance.amount / (circulation.amount * crr);
            eos_per_exchange = asset(real_type(exchange_unit.amount) * token_price, CORE_SYMBOL);
            transfer_eos += eos_per_exchange;
            circulation -= exchange_unit;
            remain_asset -= exchange_unit;
            token_balance += exchange_unit;

            eosio_assert(token_price >= real_type(0.0), "invalid token price");
        }

        eosio_assert(transfer_eos <= asset(100 * _UNIT, CORE_SYMBOL) && transfer_eos >= asset(_UNIT, CORE_SYMBOL), "卖出CGT区间为 1 - 100 EOS");
        eosio_assert(remain_asset >= asset(0, GAME_SYMBOL) && quantity >= remain_asset, "remain asset is invalid");
        eosio_assert(quantity - remain_asset == token_balance - game_itr->balance, "exchange asset is not equal");
        eosio_assert(game_itr->reserve >= transfer_eos, "insufficient reserve eos");

        user_table userinfo(_self, account);
        auto user_itr = userinfo.find(game_itr->gameid);
        if(user_itr == userinfo.end()){
            new_user(account, memo, _self);
            user_itr = userinfo.find(game_itr->gameid);
        } else {
            eosio_assert( now() >= user_itr->last_action + _ACTION_COOL_DOWN, "操作太频繁，需要时间冷却");
            userinfo.modify(user_itr, _self, [&](auto& u) {
                u.last_action = now();
            });
        }

        asset fee = transfer_eos;
        fee.amount = (fee.amount + 99) / 100; /// 1% fee (first round up)
        asset quant_after_fee = transfer_eos;
        quant_after_fee -= fee;

        _game.modify(game_itr, _self, [&](auto& g) {
            g.reserve -= transfer_eos;
            g.balance = token_balance;
            g.circulation = circulation;
        });

        collect_fee(account, fee);

        fee.amount = quant_after_fee.amount;
        if(user_itr->discount == 0){
            fee.amount = (fee.amount + 49) / 50; /// 2% fee (second round up)
        } else {
            fee.amount = (fee.amount + 99) / 100; /// 1% fee (discount half of the fee with a refer)
        }

        asset action_total_fee = fee;
        quant_after_fee -= fee;

        _game.modify(game_itr, _self, [&](auto& g) {
            g.insure += fee / 2;
            g.reward += action_total_fee - fee / 2;
        });

        if(remain_asset > asset(0, GAME_SYMBOL)){
            action(
                permission_level{_self, N(active)},
                GAME_TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, account, remain_asset, string("退回多余的CGT。 网址: http://eosplayer.one")))
            .send();
        }

        if(quant_after_fee > asset(0, CORE_SYMBOL)){
            action(
                permission_level{_self, N(active)},
                TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, account, quant_after_fee, string("卖出CGT获得EOS。 网址: http://eosplayer.one")))
            .send();
        }
    }

    void burn(account_name account, asset quantity){
        eosio_assert(quantity.amount >= _UNIT && quantity.amount <= 10000 * _UNIT, "销毁CGT的区间为 1 - 10000 CGT");
        
        auto game_itr = _game.begin();
        asset insure_balance = game_itr->insure;
        asset token_supply = game_itr->supply;
        asset token_balance = game_itr->balance;
        asset circulation = token_supply - token_balance;
        real_type token_price = real_type(insure_balance.amount) / real_type(circulation.amount);
        asset transfer_eos = asset(token_price * real_type(quantity.amount), CORE_SYMBOL);

        eosio_assert(transfer_eos <= asset(100 * _UNIT, CORE_SYMBOL) && transfer_eos >= asset(_UNIT, CORE_SYMBOL), "销毁CGT的区间为 1 - 100 EOS");
        eosio_assert(insure_balance >= transfer_eos, "insufficient insure eos");

        user_table userinfo(_self, account);
        auto user_itr = userinfo.find(game_itr->gameid);
        if(user_itr == userinfo.end()){
            new_user(account, string(""), _self);
            user_itr = userinfo.find(game_itr->gameid);
        } else {
            eosio_assert( now() >= user_itr->last_action + _ACTION_COOL_DOWN, "操作太频繁，需要时间冷却");
            userinfo.modify(user_itr, _self, [&](auto& u) {
                u.last_action = now();
            });
        }

        asset fee = transfer_eos;
        fee.amount = (fee.amount + 99) / 100; /// 1% fee (round up)
        asset quant_after_fee = transfer_eos;
        quant_after_fee -= fee;

        collect_fee(account, fee);

        _game.modify(game_itr, _self, [&](auto& g) {
            g.insure -= transfer_eos;
            g.supply -= quantity;
            g.circulation -= quantity;
            g.burn += quantity;
        });

        if(quantity > asset(0, GAME_SYMBOL)){
            action(
                permission_level{_self, N(active)},
                GAME_TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, BURN_ACCOUNT, quantity, string("销毁CGT到黑洞账号")))
            .send();
        }

        if(quant_after_fee > asset(0, CORE_SYMBOL)){
            action(
                permission_level{_self, N(active)},
                TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, account, quant_after_fee, string("销毁CGT获得EOS。 网址: http://eosplayer.one")))
            .send();
        }
    }

    void deposit(account_name account, asset quantity){
        auto game_itr = _game.begin();
        user_table userinfo(_self, account);
        auto user_itr = userinfo.find(game_itr->gameid);
        user_table fee_userinfo(_self, FEE_ACCOUNT);
        auto fee_user_itr = fee_userinfo.find(game_itr->gameid);
        if(quantity.amount >= _REFER_PRICE / 2){
            if(user_itr == userinfo.end()){
                new_user(account, string(""), _self);
                user_itr = userinfo.find(game_itr->gameid);
            }
            if(quantity.amount >= _REFER_PRICE){
                if(user_itr != userinfo.end()){
                    uint64_t refer = quantity.amount / _REFER_PRICE;
                    userinfo.modify(user_itr, _self, [&](auto& u) {
                        u.refer += refer;
                    });

                    if( now() < _GAME_PRESALE_TIME){
                        userinfo.modify(user_itr, _self, [&](auto& u) {
                            u.quota += refer;
                        });
                    }

                    fee_userinfo.modify(fee_user_itr, _self, [&](auto& u) {
                        u.refer += refer;
                    });
                }
                auto refer_itr = refers.find(account);
                if(refer_itr == refers.end() && user_itr->refer > 0){
                    refer_itr = refers.emplace(_self, [&](auto& r){
                        r.name = account;
                    });
                }
                auto fee_refer_itr = refers.find(FEE_ACCOUNT);
                if(fee_refer_itr == refers.end() && fee_user_itr->refer > 0){
                    fee_refer_itr = refers.emplace(_self, [&](auto& r){
                        r.name = FEE_ACCOUNT;
                    });
                }

                user_table next_referinfo(_self, game_itr->next_refer);
                auto next_refer_itr = next_referinfo.find(game_itr->gameid);
                if(next_refer_itr == next_referinfo.end()){
                    _game.modify(game_itr, _self, [&](auto& g){
                        g.next_refer = account;
                    });
                }
            }
        }
        
        _game.modify(game_itr, _self, [&](auto& g) {
            g.insure += quantity;
        });
    }

    void deposit_reward(asset quantity){
        auto game_itr = _game.begin();
        _game.modify(game_itr, _self, [&](auto& g) {
            g.reward += quantity;
        });
    }

    void stake(account_name account, asset quantity){
        auto game_itr = _game.begin();
        if(account == game_itr->player_one){
            // 当前是头号的用户可以无缝增加抵押CGT，稳住头号的位置
            _game.modify(game_itr, _self , [&](auto& g){
                g.staked += quantity;
            });
        } else if(quantity > game_itr->staked){
            // 取代当前的头号的位置，并且奖励周期刷新
            unstake(game_itr->player_one);
            _game.modify(game_itr, _self, [&](auto& g){
                g.staked = quantity;
                g.player_one = account;
                g.reward_time = now() + _REWARD_COOL_DOWN;
            });
        } else {
            eosio_assert(false, "需要抵押更多的CGT才能超越当前的头号");
        }
    }

    void unstake(account_name account){
        auto game_itr = _game.begin();
        eosio_assert(game_itr->player_one == account, "can only unstake player one's token");
        asset staked = game_itr->staked;
        _game.modify(game_itr, _self, [&](auto& g){
            g.staked = asset(0, GAME_SYMBOL);
            g.player_one = FEE_ACCOUNT;
        });
        if(staked >= asset(1000 * _UNIT, GAME_SYMBOL)){
            // 争取头号的用户将有10%的抵押CGT作为手续费，解除抵押的时候收取
            asset fee = staked / 10;
            staked -= fee;
            _game.modify(game_itr, _self, [&](auto& g) {
                g.supply -= fee;
                g.circulation -= fee;
                g.burn += fee;
            });

            action(
                permission_level{_self, N(active)},
                GAME_TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, BURN_ACCOUNT, fee, string("解除抵押将损失百分之十的抵押CGT")))
            .send();

            action(
                permission_level{_self, N(active)},
                GAME_TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, account, staked, string("可能有其他玩家抵押超越了您，您已经不再是头号。 网址: http://eosplayer.one")))
            .send();

            action(
                permission_level{_self, N(active)},
                TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, account, asset(1ll, CORE_SYMBOL), string("可能有其他玩家抵押超越了您，您已经不再是头号。 网址: http://eosplayer.one")))
            .send();
            
            claim_reward(game_itr->player_one);
        } else if(staked > asset(0, GAME_SYMBOL)){
            _game.modify(game_itr, _self, [&](auto& g) {
                g.supply -= staked;
                g.circulation -= staked;
                g.burn += staked;
            });

            action(
                permission_level{_self, N(active)},
                GAME_TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, BURN_ACCOUNT, staked, string("抵押CGT太少，将全部损失")))
            .send();
        }
    }

    void claim_reward(account_name account){
        auto game_itr = _game.begin();
        eosio_assert(game_itr->player_one == account, "only player one can claim the reward");
        asset reward = game_itr->reward;
        // 每一个奖励周期（24小时），头号都能够获得手续费奖池的10%，前提是发送金额大于10EOS
        reward.amount = reward.amount / 10;
        if( now() >= game_itr->reward_time && reward >= asset(10 * _UNIT, CORE_SYMBOL)){
            _game.modify(game_itr, _self, [&](auto& g){
                g.reward -= reward;
                g.reward_time = now() + _REWARD_COOL_DOWN;
            });
            action(
                permission_level{_self, N(active)},
                TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, account, reward, string("头号奖励。 网址: http://eosplayer.one")))
            .send();
        }
    }

    void new_user(account_name account, string parent_str, account_name ram_payer){
        auto game_itr = _game.begin();
        user_table userinfo(_self, account);
        auto user_itr = userinfo.find(game_itr->gameid);
        if(user_itr != userinfo.end()) return;

        uint32_t discount = 0;
        auto parent = string_to_name(parent_str.c_str());
        user_table parentinfo(_self, parent);
        auto parent_itr = parentinfo.find(game_itr->gameid);
        //新用户缺失推荐人的情况下将均匀分配到购买了邀请码的用户
        if(parent_itr == parentinfo.end() || parent_itr->refer <= 0 || account == parent){
            //如果没有用户购买邀请码，新用户的上级将默认分配到FEE_ACCOUNT，否则按照推荐人队列依次分配
            parent = FEE_ACCOUNT;
            user_table next_referinfo(_self, game_itr->next_refer);
            auto next_refer_itr = next_referinfo.find(game_itr->gameid);
            if(refers.begin() != refers.end() && next_refer_itr != next_referinfo.end()){
                auto refer_itr = refers.find(game_itr->next_refer);
                if(refer_itr != refers.end()){
                    user_table refer_userinfo(_self, refer_itr->name);
                    auto refer_user_itr = refer_userinfo.find(game_itr->gameid);
                    if(refer_user_itr != refer_userinfo.end()){
                        parent = refer_user_itr->name;
                        if(refer_user_itr->refer <= 1){
                            refer_itr = refers.erase(refer_itr);
                        } else {
                            refer_itr ++;
                        }
                        if(refers.begin() == refers.end()){
                            _game.modify(game_itr, _self, [&](auto& g){
                                g.next_refer = BURN_ACCOUNT;
                            });
                        } else if(refer_itr == refers.end()) {
                            _game.modify(game_itr, _self, [&](auto& g){
                                g.next_refer = refers.begin()->name;
                            });
                        } else {
                            _game.modify(game_itr, _self, [&](auto& g){
                                g.next_refer = refer_itr->name;
                            });
                        }
                    }
                }
            }
        } else {
            if( now() < _GAME_PRESALE_TIME ){
                parentinfo.modify(parent_itr, ram_payer, [&](auto& u){
                    u.quota += 1;
                });
            }
        }

        user_table parent_info(_self, parent);
        parent_itr = parent_info.find(game_itr->gameid);
        //分配到邀请码的新用户交易手续费减少50%
        if(parent_itr->refer > 0){
            discount = 1;
            parent_info.modify(parent_itr, ram_payer, [&](auto& u){
                u.refer --;
            });
        }

        userinfo.emplace(ram_payer, [&](auto& u) {
            u.gameid = game_itr->gameid;
            u.name = account;
            u.parent = parent;
            u.discount = discount;
            u.last_action = now();
        });
    }

    void collect_fee(account_name account, asset fee){
        //邀请奖励累积到合约，统一赎回
        if (fee.amount > 0){
             //邀请奖励金池与主奖金池隔离
            auto game_itr = _game.begin();
            _game.modify(game_itr, _self, [&](auto& g){
                g.fee += fee;
            });

            asset refer_fee = fee;
            refer_fee = fee / 2;
            fee -= refer_fee;

            user_table userinfo(_self, account);
            auto user_itr = userinfo.find(game_itr->gameid);
            if(user_itr == userinfo.end()) return;
            user_table parentinfo(_self, user_itr->parent);
            auto parent_itr = parentinfo.find(game_itr->gameid);
            if(parent_itr == parentinfo.end()) return;
            //邀请奖励回馈直接上级50%
            parentinfo.modify(parent_itr, _self, [&](auto& u){
                u.reward += fee;
            });

            if (refer_fee.amount > 0)
            {
                user_table second_parentinfo(_self, parent_itr->parent);
                auto second_parent_itr = second_parentinfo.find(game_itr->gameid);
                if(second_parent_itr == second_parentinfo.end()) return;
                //邀请奖励二级回馈50%
                second_parentinfo.modify(second_parent_itr, _self, [&](auto& u){
                    u.reward += refer_fee;
                });
            }
        }
    }

    void claim_fee(account_name account){
        auto game_itr = _game.begin();
        user_table userinfo(_self, account);
        auto user_itr = userinfo.find(game_itr->gameid);
        if(user_itr == userinfo.end()) return;
        //邀请奖励累积到1EOS及以上才能够赎回
        if(user_itr->reward >= asset(_UNIT, CORE_SYMBOL)){
            asset reward = user_itr->reward;
            userinfo.modify(user_itr, _self, [&](auto& u){
                u.reward = asset(0, CORE_SYMBOL);
            });
            //邀请奖励金池与主奖金池隔离
            _game.modify(game_itr, _self, [&](auto& g){
                g.fee -= reward;
            });
            //邀请奖励金池与主奖金池隔离
            eosio_assert(game_itr->fee >= asset(0, CORE_SYMBOL), "shit happens");
            action(
                permission_level{_self, N(active)},
                TOKEN_CONTRACT, N(transfer),
                make_tuple(_self, user_itr->name, reward, string("获得邀请奖励。 网址: http://eosplayer.one")))
            .send();
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
        real_type UNIT(_UNIT);
        real_type E(2.71828182845904);

        real_type X(circulation.amount);
        X = X / UNIT;
        
        real_type R = ONE / (ONE + pow(E, (X - L) / D)) * A + B;
        eosio_assert(R >= B && R <= B + A, "shit happens");
        return R / H;
    }

    string name_to_string(account_name name){
        static const char* charmap = ".12345abcdefghijklmnopqrstuvwxyz";
        string str(13,'.');
        uint64_t tmp = name;
        for( uint32_t i = 0; i <= 12; ++i ) {
            char c = charmap[tmp & (i == 0 ? 0x0f : 0x1f)];
            str[12-i] = c;
            tmp >>= (i == 0 ? 4 : 5);
        }
        const auto last = str.find_last_not_of('.');
        if (last != string::npos)
            str = str.substr(0, last + 1);
        return str;
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
        asset staked = asset(0, GAME_SYMBOL);
        asset fee = asset(0, CORE_SYMBOL);
        asset reward = asset(0, CORE_SYMBOL);
        account_name next_refer = FEE_ACCOUNT;
        account_name player_one = FEE_ACCOUNT;
        uint64_t start_time;
        uint64_t reward_time;

        uint64_t primary_key() const { return gameid; }
        EOSLIB_SERIALIZE(game, (gameid)(reserve)(insure)(max_supply)(supply)(balance)(circulation)(burn)(staked)(fee)(reward)(next_refer)(player_one)(start_time)(reward_time))
    };
    typedef eosio::multi_index<N(game), game> game_table;
    game_table _game;

    // @abi table userinfo i64
    struct user{
        account_name gameid;
        account_name name;
        account_name parent;
        asset reward = asset(0, CORE_SYMBOL);
        uint64_t last_action;
        uint64_t refer = 0;
        uint64_t quota = 0;
        uint32_t discount = 0;

        uint64_t primary_key() const { return gameid; }
        EOSLIB_SERIALIZE(user, (gameid)(name)(parent)(reward)(last_action)(refer)(quota)(discount))
    };
    typedef eosio::multi_index<N(userinfo), user> user_table;
    // user_table userinfo;

    // @abi table refers i64
    struct refer{
        account_name name;
        uint64_t primary_key() const { return name; }
        EOSLIB_SERIALIZE(refer, (name))
    };
    typedef eosio::multi_index<N(refers), refer> refer_table;
    refer_table refers;

    // @abi table invitations i64
    struct invitation{
        account_name to;
        account_name from;
        uint64_t primary_key() const { return to; }
        EOSLIB_SERIALIZE(invitation, (to)(from))
    };
    typedef eosio::multi_index<N(invitations), invitation> invitation_table;
    invitation_table invitations;
};

#define EOSIO_ABI_EX( TYPE, MEMBERS ) \
extern "C" { \
    void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
        auto self = receiver; \
        if(action == N(onerror)) { \
            /* onerror is only valid if it is for the "eosio" code account and authorized by "eosio"'s "active permission */ \
            eosio_assert(code == N(eosio), "onerror action's are only valid from the \"eosio\" system account"); \
        } \
        if(action == N(transfer)) { \
            auto _action = N(onerror); \
            if(code == TOKEN_CONTRACT) { \
                _action = N(eosio_token_transfer); \
            } else if(code == GAME_TOKEN_CONTRACT) { \
                _action = N(game_token_transfer); \
            } \
            if(_action == N(onerror)) { \
                eosio_assert(false, "action from this code is denied"); \
            } \
            TYPE thiscontract( self ); \
            switch( _action ) { \
                EOSIO_API( TYPE, MEMBERS ) \
            } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
        } \
    } \
} \

EOSIO_ABI_EX(playerone, (eosio_token_transfer)(game_token_transfer))