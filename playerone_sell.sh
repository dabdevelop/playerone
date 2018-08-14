#! /bin/bash

for ((i=0; i<1; ++i)) 
do  
    cleos push action eosio.token transfer '["testuseraaaa", "oneplayerone", "1.1111 EOS", "testuseraaaa"]' -p testuseraaaa
    cleos push action eosio.token transfer '["testuseraaab", "oneplayerone", "1.1111 EOS", "testuseraaaa"]' -p testuseraaab
    cleos push action eosio.token transfer '["testuseraaac", "oneplayerone", "1.1111 EOS", "testuseraaab"]' -p testuseraaac
    cleos push action eosio.token transfer '["testuseraaad", "oneplayerone", "1.1111 EOS", ""]' -p testuseraaad
    cleos push action eosio.token transfer '["testuseraaae", "oneplayerone", "1.1111 EOS", "testuseraaaa"]' -p testuseraaae
    cleos push action eosio.token transfer '["testuseraaaf", "oneplayerone", "1.1111 EOS", "testuseraaaa"]' -p testuseraaaf
    cleos push action eosio.token transfer '["testuseraaag", "oneplayerone", "1.1111 EOS", "testuseraaab"]' -p testuseraaag
    cleos push action eosio.token transfer '["testuseraaah", "oneplayerone", "1.1111 EOS", ""]' -p testuseraaah
    cleos push action eosio.token transfer '["testuseraaai", "oneplayerone", "1.1111 EOS", "testuseraaab"]' -p testuseraaai
    cleos push action eosio.token transfer '["testuseraaaj", "oneplayerone", "1.1111 EOS", ""]' -p testuseraaaj

    cleos push action playeroneiss transfer '["testuseraaaa", "oneplayerone", "111.1111 CGT", ""]' -p testuseraaaa
    cleos push action playeroneiss transfer '["testuseraaab", "oneplayerone", "111.1111 CGT", ""]' -p testuseraaab
    cleos push action playeroneiss transfer '["testuseraaac", "oneplayerone", "111.1111 CGT", ""]' -p testuseraaac
    cleos push action playeroneiss transfer '["testuseraaad", "oneplayerone", "111.1111 CGT", ""]' -p testuseraaad
    cleos push action playeroneiss transfer '["testuseraaae", "oneplayerone", "111.1111 CGT", ""]' -p testuseraaae
    cleos push action playeroneiss transfer '["testuseraaaf", "oneplayerone", "111.1111 CGT", ""]' -p testuseraaaf
    cleos push action playeroneiss transfer '["testuseraaag", "oneplayerone", "111.1111 CGT", ""]' -p testuseraaag
    cleos push action playeroneiss transfer '["testuseraaah", "oneplayerone", "111.1111 CGT", ""]' -p testuseraaah
    cleos push action playeroneiss transfer '["testuseraaai", "oneplayerone", "111.1111 CGT", ""]' -p testuseraaai
    cleos push action playeroneiss transfer '["testuseraaaj", "oneplayerone", "111.1111 CGT", ""]' -p testuseraaaj

done

cleos get table oneplayerone oneplayerone game
cleos get table oneplayerone oneplayerone users