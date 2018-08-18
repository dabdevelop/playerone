#! /bin/bash

EOS_RPC=https://mainnet-eos.token.im
alias cleos_mainnet="cleos -u $EOS_RPC"

# assume you have imported the officials' keys

# cleos set contract oneplayerone ./build/playerone -p oneplayerone

# cleos set code oneplayerone ./build/playerone/playerone.wasm -p oneplayerone
# cleos set abi oneplayerone ./build/playerone/playerone.abi -p oneplayerone

cleos_mainnet set contract playeroneiss ./build/eosio.token -p playeroneiss

# cleos push action playeroneiss create '[ "oneplayerone", "10000000.0000 CGT"]' -p playeroneiss

# cleos set account permission oneplayerone active '{"threshold": 1,"keys": [{"key": "EOS4wen3kfDXSHEb4nYmzDkcfvoPZb2jyoRS6mb6EbGJgM5Apu6Go","weight": 1}],"accounts": [{"permission":{"actor":"oneplayerone","permission":"eosio.code"},"weight":1}]}' owner -p oneplayerone

# follow test must throw
# cleos push action playeroneiss create '[ "playeronefee", "10000000.0000 EOS"]' -p playeroneiss
# cleos push action playeroneiss issue '[ "playeronefee", "100000.0000 EOS"]' -p playeronefee
# cleos push action playeroneiss transfer '["playeronefee", "oneplayerone", "10.1111 EOS", "testuseraaaa"]' -p playeronefee
