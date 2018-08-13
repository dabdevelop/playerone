# blackholeeos = EOS5PW5ZchfiCF2uPuFUEt8xseS1wiNqBjCHQTYxXT3unLNamrpwi=KEY:5KaVNWNF2LtdxBT8fGUV81yXZjKiu7qdR28myrUKEXcem8gc1qy
cleos create account eosio blackholeeos EOS5PW5ZchfiCF2uPuFUEt8xseS1wiNqBjCHQTYxXT3unLNamrpwi EOS5PW5ZchfiCF2uPuFUEt8xseS1wiNqBjCHQTYxXT3unLNamrpwi
# playeronefee = EOS7AH473AJ1Dix3x7SjMU9tjsG7Fo79JZq9GuZjL9f5FCu6rUhza=KEY:5KNGa5bkAW8sP5dqcYNHQSvAXQtsfU9yyLr87SBvE7F3RyonUZ1
cleos create account eosio playeronefee EOS7AH473AJ1Dix3x7SjMU9tjsG7Fo79JZq9GuZjL9f5FCu6rUhza EOS7AH473AJ1Dix3x7SjMU9tjsG7Fo79JZq9GuZjL9f5FCu6rUhza

# oneplayerone = EOS4wen3kfDXSHEb4nYmzDkcfvoPZb2jyoRS6mb6EbGJgM5Apu6Go=KEY:5JdthYKccniLB7w5VQegU4HkERpzMD9yQ1hK3AJvduZKAr9EoeT
cleos create account eosio oneplayerone EOS4wen3kfDXSHEb4nYmzDkcfvoPZb2jyoRS6mb6EbGJgM5Apu6Go EOS4wen3kfDXSHEb4nYmzDkcfvoPZb2jyoRS6mb6EbGJgM5Apu6Go
# cleos system newaccount eosio oneplayerone EOS4wen3kfDXSHEb4nYmzDkcfvoPZb2jyoRS6mb6EbGJgM5Apu6Go EOS4wen3kfDXSHEb4nYmzDkcfvoPZb2jyoRS6mb6EbGJgM5Apu6Go --stake-net "10.0000 SYS" --stake-cpu "10.0000 SYS" --buy-ram-kbytes 1000

# playeroneiss = EOS8cgtXLQLtAzRyNytCLdvZGMaixKrt9nmoUkBY4MfULriSg4Uzo=KEY:5K5rvrLPKP9tNkLngX9vJcLDYeh6nAurakb2Xwc2CpaRaAwazZ4
cleos create account eosio playeroneiss EOS8cgtXLQLtAzRyNytCLdvZGMaixKrt9nmoUkBY4MfULriSg4Uzo EOS8cgtXLQLtAzRyNytCLdvZGMaixKrt9nmoUkBY4MfULriSg4Uzo
# cleos system newaccount eosio playeroneiss EOS8cgtXLQLtAzRyNytCLdvZGMaixKrt9nmoUkBY4MfULriSg4Uzo EOS8cgtXLQLtAzRyNytCLdvZGMaixKrt9nmoUkBY4MfULriSg4Uzo --stake-net "10.0000 SYS" --stake-cpu "10.0000 SYS" --buy-ram-kbytes 10000

cleos set contract oneplayerone ./build/playerone -p oneplayerone
cleos set contract playeroneiss ./build/eosio.token -p playeroneiss
cleos push action playeroneiss create '[ "oneplayerone", "3000000.0000 CGT"]' -p playeroneiss

cleos set account permission oneplayerone active '{"threshold": 1,"keys": [{"key": "EOS4wen3kfDXSHEb4nYmzDkcfvoPZb2jyoRS6mb6EbGJgM5Apu6Go","weight": 1}],"accounts": [{"permission":{"actor":"oneplayerone","permission":"eosio.code"},"weight":1}]}' owner -p oneplayerone

cleos push action eosio.token transfer '["testuseraaaa", "oneplayerone", "1.0000 EOS", ""]' -p testuseraaaa