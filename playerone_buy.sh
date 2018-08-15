#! /bin/bash

cleos push action eosio.token transfer '["testuseraaaa", "oneplayerone", "3.0000 EOS", "deposit"]' -p testuseraaaa

for ((i=0; i<1; ++i)) 
do  
    cleos push action eosio.token transfer '["testuseraaaa", "oneplayerone", "99.9999 EOS", "testuseraaaa"]' -p testuseraaaa
    cleos push action eosio.token transfer '["testuseraaab", "oneplayerone", "99.9999 EOS", "testuseraaaa"]' -p testuseraaab
    cleos push action eosio.token transfer '["testuseraaac", "oneplayerone", "99.9999 EOS", "testuseraaab"]' -p testuseraaac
    cleos push action eosio.token transfer '["testuseraaad", "oneplayerone", "99.9999 EOS", ""]' -p testuseraaad
    cleos push action eosio.token transfer '["testuseraaae", "oneplayerone", "99.9999 EOS", ""]' -p testuseraaae
    cleos push action eosio.token transfer '["testuseraaaf", "oneplayerone", "99.9999 EOS", ""]' -p testuseraaaf
    cleos push action eosio.token transfer '["testuseraaag", "oneplayerone", "99.9999 EOS", ""]' -p testuseraaag
    cleos push action eosio.token transfer '["testuseraaah", "oneplayerone", "99.9999 EOS", ""]' -p testuseraaah
    cleos push action eosio.token transfer '["testuseraaai", "oneplayerone", "99.9999 EOS", ""]' -p testuseraaai
    cleos push action eosio.token transfer '["testuseraaaj", "oneplayerone", "99.9999 EOS", ""]' -p testuseraaaj

    cleos push action playeroneiss transfer '["testuseraaaa", "oneplayerone", "11.1111 CGT", ""]' -p testuseraaaa
    cleos push action playeroneiss transfer '["testuseraaab", "oneplayerone", "11.1111 CGT", ""]' -p testuseraaab
    cleos push action playeroneiss transfer '["testuseraaac", "oneplayerone", "11.1111 CGT", ""]' -p testuseraaac
    cleos push action playeroneiss transfer '["testuseraaad", "oneplayerone", "11.1111 CGT", ""]' -p testuseraaad
    cleos push action playeroneiss transfer '["testuseraaae", "oneplayerone", "11.1111 CGT", ""]' -p testuseraaae
    cleos push action playeroneiss transfer '["testuseraaaf", "oneplayerone", "11.1111 CGT", ""]' -p testuseraaaf
    cleos push action playeroneiss transfer '["testuseraaag", "oneplayerone", "11.1111 CGT", ""]' -p testuseraaag
    cleos push action playeroneiss transfer '["testuseraaah", "oneplayerone", "11.1111 CGT", ""]' -p testuseraaah
    cleos push action playeroneiss transfer '["testuseraaai", "oneplayerone", "11.1111 CGT", ""]' -p testuseraaai
    cleos push action playeroneiss transfer '["testuseraaaj", "oneplayerone", "11.1111 CGT", ""]' -p testuseraaaj

done

cleos get table oneplayerone oneplayerone game
cleos get table oneplayerone oneplayerone users
cleos get table oneplayerone oneplayerone refers