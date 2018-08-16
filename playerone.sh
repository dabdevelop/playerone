#! /bin/bash

cleos push action eosio.token transfer '["testuseraaaa", "oneplayerone", "2.0000 EOS", "deposit"]' -p testuseraaaa
cleos push action eosio.token transfer '["testuseraaad", "oneplayerone", "2.0000 EOS", "deposit"]' -p testuseraaad

for ((i=0; i<20; ++i)) 
do  
    cleos push action eosio.token transfer '["testuseraaaa", "oneplayerone", "100.0000 EOS", "testuseraaaa"]' -p testuseraaaa
    cleos push action eosio.token transfer '["testuseraaab", "oneplayerone", "100.0000 EOS", "testuseraaaa"]' -p testuseraaab
    cleos push action eosio.token transfer '["testuseraaac", "oneplayerone", "100.0000 EOS", "testuseraaab"]' -p testuseraaac
    cleos push action eosio.token transfer '["testuseraaad", "oneplayerone", "100.0000 EOS", ""]' -p testuseraaad
    cleos push action eosio.token transfer '["testuseraaae", "oneplayerone", "100.0000 EOS", ""]' -p testuseraaae
    cleos push action eosio.token transfer '["testuseraaaf", "oneplayerone", "100.0000 EOS", ""]' -p testuseraaaf
    cleos push action eosio.token transfer '["testuseraaag", "oneplayerone", "100.0000 EOS", ""]' -p testuseraaag
    cleos push action eosio.token transfer '["testuseraaah", "oneplayerone", "100.0000 EOS", ""]' -p testuseraaah
    cleos push action eosio.token transfer '["testuseraaai", "oneplayerone", "100.0000 EOS", ""]' -p testuseraaai
    cleos push action eosio.token transfer '["testuseraaaj", "oneplayerone", "100.0000 EOS", ""]' -p testuseraaaj

    cleos push action playeroneiss transfer '["testuseraaaa", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaaa
    cleos push action playeroneiss transfer '["testuseraaab", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaab
    cleos push action playeroneiss transfer '["testuseraaac", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaac
    cleos push action playeroneiss transfer '["testuseraaad", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaad
    cleos push action playeroneiss transfer '["testuseraaae", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaae
    cleos push action playeroneiss transfer '["testuseraaaf", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaaf
    cleos push action playeroneiss transfer '["testuseraaag", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaag
    cleos push action playeroneiss transfer '["testuseraaah", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaah
    cleos push action playeroneiss transfer '["testuseraaai", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaai
    cleos push action playeroneiss transfer '["testuseraaaj", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaaj
    cleos push action playeroneiss transfer '["testuseraaaj", "oneplayerone", "1000.0000 CGT", "burn"]' -p testuseraaaj

done

for ((i=0; i<15; ++i)) 
do  
    cleos push action eosio.token transfer '["testuseraaaa", "oneplayerone", "50.0000 EOS", "testuseraaaa"]' -p testuseraaaa
    cleos push action eosio.token transfer '["testuseraaab", "oneplayerone", "50.0000 EOS", "testuseraaaa"]' -p testuseraaab
    cleos push action eosio.token transfer '["testuseraaac", "oneplayerone", "50.0000 EOS", "testuseraaab"]' -p testuseraaac
    cleos push action eosio.token transfer '["testuseraaad", "oneplayerone", "50.0000 EOS", ""]' -p testuseraaad
    cleos push action eosio.token transfer '["testuseraaae", "oneplayerone", "50.0000 EOS", ""]' -p testuseraaae
    cleos push action eosio.token transfer '["testuseraaaf", "oneplayerone", "50.0000 EOS", ""]' -p testuseraaaf
    cleos push action eosio.token transfer '["testuseraaag", "oneplayerone", "50.0000 EOS", ""]' -p testuseraaag
    cleos push action eosio.token transfer '["testuseraaah", "oneplayerone", "50.0000 EOS", ""]' -p testuseraaah
    cleos push action eosio.token transfer '["testuseraaai", "oneplayerone", "50.0000 EOS", ""]' -p testuseraaai
    cleos push action eosio.token transfer '["testuseraaaj", "oneplayerone", "50.0000 EOS", ""]' -p testuseraaaj

    cleos push action playeroneiss transfer '["testuseraaaa", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaaa
    cleos push action playeroneiss transfer '["testuseraaab", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaab
    cleos push action playeroneiss transfer '["testuseraaac", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaac
    cleos push action playeroneiss transfer '["testuseraaad", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaad
    cleos push action playeroneiss transfer '["testuseraaae", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaae
    cleos push action playeroneiss transfer '["testuseraaaf", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaaf
    cleos push action playeroneiss transfer '["testuseraaag", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaag
    cleos push action playeroneiss transfer '["testuseraaah", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaah
    cleos push action playeroneiss transfer '["testuseraaai", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaai
    cleos push action playeroneiss transfer '["testuseraaaj", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaaj
    cleos push action playeroneiss transfer '["testuseraaaj", "oneplayerone", "1000.0000 CGT", "burn"]' -p testuseraaaj
done

for ((i=0; i<20; ++i)) 
do  
    cleos push action eosio.token transfer '["testuseraaaa", "oneplayerone", "50.0000 EOS", "testuseraaaa"]' -p testuseraaaa
    cleos push action eosio.token transfer '["testuseraaab", "oneplayerone", "50.0000 EOS", "testuseraaaa"]' -p testuseraaab
    cleos push action eosio.token transfer '["testuseraaac", "oneplayerone", "50.0000 EOS", "testuseraaab"]' -p testuseraaac
    cleos push action eosio.token transfer '["testuseraaad", "oneplayerone", "50.0000 EOS", ""]' -p testuseraaad
    cleos push action eosio.token transfer '["testuseraaae", "oneplayerone", "50.0000 EOS", ""]' -p testuseraaae
    cleos push action eosio.token transfer '["testuseraaaf", "oneplayerone", "50.0000 EOS", ""]' -p testuseraaaf
    cleos push action eosio.token transfer '["testuseraaag", "oneplayerone", "50.0000 EOS", ""]' -p testuseraaag
    cleos push action eosio.token transfer '["testuseraaah", "oneplayerone", "50.0000 EOS", ""]' -p testuseraaah
    cleos push action eosio.token transfer '["testuseraaai", "oneplayerone", "50.0000 EOS", ""]' -p testuseraaai
    cleos push action eosio.token transfer '["testuseraaaj", "oneplayerone", "50.0000 EOS", ""]' -p testuseraaaj

    cleos push action playeroneiss transfer '["testuseraaaa", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaaa
    cleos push action playeroneiss transfer '["testuseraaab", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaab
    cleos push action playeroneiss transfer '["testuseraaac", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaac
    cleos push action playeroneiss transfer '["testuseraaad", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaad
    cleos push action playeroneiss transfer '["testuseraaae", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaae
    cleos push action playeroneiss transfer '["testuseraaaf", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaaf
    cleos push action playeroneiss transfer '["testuseraaag", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaag
    cleos push action playeroneiss transfer '["testuseraaah", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaah
    cleos push action playeroneiss transfer '["testuseraaai", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaai
    cleos push action playeroneiss transfer '["testuseraaaj", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaaj
    cleos push action playeroneiss transfer '["testuseraaaj", "oneplayerone", "1000.0000 CGT", "burn"]' -p testuseraaaj
done

    


cleos get table oneplayerone oneplayerone users
cleos get table oneplayerone oneplayerone refers

cleos get table oneplayerone oneplayerone game
echo cleos get currency balance playeroneiss oneplayerone CGT
cleos get currency balance playeroneiss oneplayerone CGT
echo cleos get currency balance playeroneiss blackholeeos CGT
cleos get currency balance playeroneiss blackholeeos CGT
echo cleos get currency balance eosio.token oneplayerone EOS
cleos get currency balance eosio.token oneplayerone EOS
echo cleos get currency stats playeroneiss CGT
cleos get currency stats playeroneiss CGT