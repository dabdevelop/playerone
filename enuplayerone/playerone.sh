#! /bin/bash

enucli push action enu.token transfer '["testuseraaaa", "oneplayerone", "2.0000 ENU", "deposit"]' -p testuseraaaa
enucli push action enu.token transfer '["testuseraaad", "oneplayerone", "2.0000 ENU", "deposit"]' -p testuseraaad

for ((i=0; i<20; ++i)) 
do  
    enucli push action enu.token transfer '["testuseraaaa", "oneplayerone", "100.0000 ENU", "testuseraaaa"]' -p testuseraaaa
    enucli push action enu.token transfer '["testuseraaab", "oneplayerone", "100.0000 ENU", "testuseraaaa"]' -p testuseraaab
    enucli push action enu.token transfer '["testuseraaac", "oneplayerone", "100.0000 ENU", "testuseraaab"]' -p testuseraaac
    enucli push action enu.token transfer '["testuseraaad", "oneplayerone", "100.0000 ENU", ""]' -p testuseraaad
    enucli push action enu.token transfer '["testuseraaae", "oneplayerone", "100.0000 ENU", ""]' -p testuseraaae
    enucli push action enu.token transfer '["testuseraaaf", "oneplayerone", "100.0000 ENU", ""]' -p testuseraaaf
    enucli push action enu.token transfer '["testuseraaag", "oneplayerone", "100.0000 ENU", ""]' -p testuseraaag
    enucli push action enu.token transfer '["testuseraaah", "oneplayerone", "100.0000 ENU", ""]' -p testuseraaah
    enucli push action enu.token transfer '["testuseraaai", "oneplayerone", "100.0000 ENU", ""]' -p testuseraaai
    enucli push action enu.token transfer '["testuseraaaj", "oneplayerone", "100.0000 ENU", ""]' -p testuseraaaj

    enucli push action playeroneiss transfer '["testuseraaaa", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaaa
    enucli push action playeroneiss transfer '["testuseraaab", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaab
    enucli push action playeroneiss transfer '["testuseraaac", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaac
    enucli push action playeroneiss transfer '["testuseraaad", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaad
    enucli push action playeroneiss transfer '["testuseraaae", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaae
    enucli push action playeroneiss transfer '["testuseraaaf", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaaf
    enucli push action playeroneiss transfer '["testuseraaag", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaag
    enucli push action playeroneiss transfer '["testuseraaah", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaah
    enucli push action playeroneiss transfer '["testuseraaai", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaai
    enucli push action playeroneiss transfer '["testuseraaaj", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaaj
    enucli push action playeroneiss transfer '["testuseraaaj", "oneplayerone", "1000.0000 CGT", "burn"]' -p testuseraaaj

done

for ((i=0; i<15; ++i)) 
do  
    enucli push action enu.token transfer '["testuseraaaa", "oneplayerone", "50.0000 ENU", "testuseraaaa"]' -p testuseraaaa
    enucli push action enu.token transfer '["testuseraaab", "oneplayerone", "50.0000 ENU", "testuseraaaa"]' -p testuseraaab
    enucli push action enu.token transfer '["testuseraaac", "oneplayerone", "50.0000 ENU", "testuseraaab"]' -p testuseraaac
    enucli push action enu.token transfer '["testuseraaad", "oneplayerone", "50.0000 ENU", ""]' -p testuseraaad
    enucli push action enu.token transfer '["testuseraaae", "oneplayerone", "50.0000 ENU", ""]' -p testuseraaae
    enucli push action enu.token transfer '["testuseraaaf", "oneplayerone", "50.0000 ENU", ""]' -p testuseraaaf
    enucli push action enu.token transfer '["testuseraaag", "oneplayerone", "50.0000 ENU", ""]' -p testuseraaag
    enucli push action enu.token transfer '["testuseraaah", "oneplayerone", "50.0000 ENU", ""]' -p testuseraaah
    enucli push action enu.token transfer '["testuseraaai", "oneplayerone", "50.0000 ENU", ""]' -p testuseraaai
    enucli push action enu.token transfer '["testuseraaaj", "oneplayerone", "50.0000 ENU", ""]' -p testuseraaaj

    enucli push action playeroneiss transfer '["testuseraaaa", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaaa
    enucli push action playeroneiss transfer '["testuseraaab", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaab
    enucli push action playeroneiss transfer '["testuseraaac", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaac
    enucli push action playeroneiss transfer '["testuseraaad", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaad
    enucli push action playeroneiss transfer '["testuseraaae", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaae
    enucli push action playeroneiss transfer '["testuseraaaf", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaaf
    enucli push action playeroneiss transfer '["testuseraaag", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaag
    enucli push action playeroneiss transfer '["testuseraaah", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaah
    enucli push action playeroneiss transfer '["testuseraaai", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaai
    enucli push action playeroneiss transfer '["testuseraaaj", "oneplayerone", "500.0000 CGT", ""]' -p testuseraaaj
    enucli push action playeroneiss transfer '["testuseraaaj", "oneplayerone", "1000.0000 CGT", "burn"]' -p testuseraaaj
done

for ((i=0; i<20; ++i)) 
do  
    enucli push action enu.token transfer '["testuseraaaa", "oneplayerone", "50.0000 ENU", "testuseraaaa"]' -p testuseraaaa
    enucli push action enu.token transfer '["testuseraaab", "oneplayerone", "50.0000 ENU", "testuseraaaa"]' -p testuseraaab
    enucli push action enu.token transfer '["testuseraaac", "oneplayerone", "50.0000 ENU", "testuseraaab"]' -p testuseraaac
    enucli push action enu.token transfer '["testuseraaad", "oneplayerone", "50.0000 ENU", ""]' -p testuseraaad
    enucli push action enu.token transfer '["testuseraaae", "oneplayerone", "50.0000 ENU", ""]' -p testuseraaae
    enucli push action enu.token transfer '["testuseraaaf", "oneplayerone", "50.0000 ENU", ""]' -p testuseraaaf
    enucli push action enu.token transfer '["testuseraaag", "oneplayerone", "50.0000 ENU", ""]' -p testuseraaag
    enucli push action enu.token transfer '["testuseraaah", "oneplayerone", "50.0000 ENU", ""]' -p testuseraaah
    enucli push action enu.token transfer '["testuseraaai", "oneplayerone", "50.0000 ENU", ""]' -p testuseraaai
    enucli push action enu.token transfer '["testuseraaaj", "oneplayerone", "50.0000 ENU", ""]' -p testuseraaaj

    enucli push action playeroneiss transfer '["testuseraaaa", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaaa
    enucli push action playeroneiss transfer '["testuseraaab", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaab
    enucli push action playeroneiss transfer '["testuseraaac", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaac
    enucli push action playeroneiss transfer '["testuseraaad", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaad
    enucli push action playeroneiss transfer '["testuseraaae", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaae
    enucli push action playeroneiss transfer '["testuseraaaf", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaaf
    enucli push action playeroneiss transfer '["testuseraaag", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaag
    enucli push action playeroneiss transfer '["testuseraaah", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaah
    enucli push action playeroneiss transfer '["testuseraaai", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaai
    enucli push action playeroneiss transfer '["testuseraaaj", "oneplayerone", "100.0000 CGT", ""]' -p testuseraaaj
    enucli push action playeroneiss transfer '["testuseraaaj", "oneplayerone", "1000.0000 CGT", "burn"]' -p testuseraaaj
done


# enucli transfer enu.token playeronefee "1 ENU" "test" -p enu.token
# enucli transfer playeronefee oneplayerone "0.0001 ENU" "test" -p playeronefee

# enucli push action playeroneiss transfer '["testuseraaaa", "oneplayerone", "100.0000 CGT", "stake"]' -p testuseraaaa
# enucli push action playeroneiss transfer '["testuseraaaj", "oneplayerone", "1000.0000 CGT", "stake"]' -p testuseraaaj
# enucli transfer testuseraaaa oneplayerone "0.0001 ENU" "test" -p testuseraaaa
# enucli transfer testuseraaai oneplayerone "0.0001 ENU" "test" -p testuseraaai

# enucli push action enu.token transfer '["testuseraaaa", "oneplayerone", "0.0001 ENU", "testuseraaaj"]' -p testuseraaaa

enucli get table oneplayerone testuseraaaa userinfo
enucli get table oneplayerone testuseraaab userinfo
enucli get table oneplayerone testuseraaac userinfo
enucli get table oneplayerone testuseraaad userinfo
enucli get table oneplayerone testuseraaae userinfo
enucli get table oneplayerone testuseraaaf userinfo
enucli get table oneplayerone testuseraaag userinfo
enucli get table oneplayerone testuseraaah userinfo
enucli get table oneplayerone testuseraaai userinfo
enucli get table oneplayerone testuseraaaj userinfo
enucli get table oneplayerone oneplayerone refers
enucli get table oneplayerone oneplayerone game
enucli get table oneplayerone oneplayerone invitations
echo enucli get currency balance playeroneiss oneplayerone CGT
enucli get currency balance playeroneiss oneplayerone CGT
echo enucli get currency balance playeroneiss blackholeeos CGT
enucli get currency balance playeroneiss blackholeeos CGT
echo enucli get currency balance enu.token oneplayerone ENU
enucli get currency balance enu.token oneplayerone ENU
echo enucli get currency balance enu.token playeronefee ENU
enucli get currency balance enu.token playeronefee ENU
echo enucli get currency stats playeroneiss CGT
enucli get currency stats playeroneiss CGT