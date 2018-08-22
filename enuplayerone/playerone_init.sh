#! /bin/bash

# assume you have imported the officials' keys
enucli wallet import --private-key 5KaVNWNF2LtdxBT8fGUV81yXZjKiu7qdR28myrUKEXcem8gc1qy
enucli wallet import --private-key 5KNGa5bkAW8sP5dqcYNHQSvAXQtsfU9yyLr87SBvE7F3RyonUZ1
enucli wallet import --private-key 5JdthYKccniLB7w5VQegU4HkERpzMD9yQ1hK3AJvduZKAr9EoeT
enucli wallet import --private-key 5K5rvrLPKP9tNkLngX9vJcLDYeh6nAurakb2Xwc2CpaRaAwazZ4


# blackholeeos = ENU5PW5ZchfiCF2uPuFUEt8xseS1wiNqBjCHQTYxXT3unLNamrpwi=KEY:5KaVNWNF2LtdxBT8fGUV81yXZjKiu7qdR28myrUKEXcem8gc1qy
enucli create account enumivo blackholeeos ENU5PW5ZchfiCF2uPuFUEt8xseS1wiNqBjCHQTYxXT3unLNamrpwi ENU5PW5ZchfiCF2uPuFUEt8xseS1wiNqBjCHQTYxXT3unLNamrpwi

# playeronefee = ENU7AH473AJ1Dix3x7SjMU9tjsG7Fo79JZq9GuZjL9f5FCu6rUhza=KEY:5KNGa5bkAW8sP5dqcYNHQSvAXQtsfU9yyLr87SBvE7F3RyonUZ1
enucli create account enumivo playeronefee ENU7AH473AJ1Dix3x7SjMU9tjsG7Fo79JZq9GuZjL9f5FCu6rUhza ENU7AH473AJ1Dix3x7SjMU9tjsG7Fo79JZq9GuZjL9f5FCu6rUhza

# oneplayerone = ENU4wen3kfDXSHEb4nYmzDkcfvoPZb2jyoRS6mb6EbGJgM5Apu6Go=KEY:5JdthYKccniLB7w5VQegU4HkERpzMD9yQ1hK3AJvduZKAr9EoeT
enucli create account enumivo oneplayerone ENU4wen3kfDXSHEb4nYmzDkcfvoPZb2jyoRS6mb6EbGJgM5Apu6Go ENU4wen3kfDXSHEb4nYmzDkcfvoPZb2jyoRS6mb6EbGJgM5Apu6Go
# enucli system newaccount enumivo oneplayerone ENU4wen3kfDXSHEb4nYmzDkcfvoPZb2jyoRS6mb6EbGJgM5Apu6Go ENU4wen3kfDXSHEb4nYmzDkcfvoPZb2jyoRS6mb6EbGJgM5Apu6Go --stake-net "10.0000 SYS" --stake-cpu "10.0000 SYS" --buy-ram-kbytes 1000

# playeroneiss = ENU8cgtXLQLtAzRyNytCLdvZGMaixKrt9nmoUkBY4MfULriSg4Uzo=KEY:5K5rvrLPKP9tNkLngX9vJcLDYeh6nAurakb2Xwc2CpaRaAwazZ4
enucli create account enumivo playeroneiss ENU8cgtXLQLtAzRyNytCLdvZGMaixKrt9nmoUkBY4MfULriSg4Uzo ENU8cgtXLQLtAzRyNytCLdvZGMaixKrt9nmoUkBY4MfULriSg4Uzo
# enucli system newaccount enumivo playeroneiss ENU8cgtXLQLtAzRyNytCLdvZGMaixKrt9nmoUkBY4MfULriSg4Uzo ENU8cgtXLQLtAzRyNytCLdvZGMaixKrt9nmoUkBY4MfULriSg4Uzo --stake-net "10.0000 SYS" --stake-cpu "10.0000 SYS" --buy-ram-kbytes 10000

# enucli set contract oneplayerone ./build/playerone -p oneplayerone
enucli set code oneplayerone ./playerone.wasm -p oneplayerone
enucli set abi oneplayerone ./playerone.abi -p oneplayerone

enucli set contract playeroneiss ./enu.token -p playeroneiss
enucli push action playeroneiss create '[ "oneplayerone", "10000000.0000 CGT"]' -p playeroneiss

enucli set account permission oneplayerone active '{"threshold": 1,"keys": [{"key": "ENU4wen3kfDXSHEb4nYmzDkcfvoPZb2jyoRS6mb6EbGJgM5Apu6Go","weight": 1}],"accounts": [{"permission":{"actor":"oneplayerone","permission":"enumivo.code"},"weight":1}]}' owner -p oneplayerone

# follow test must throw
# enucli push action playeroneiss create '[ "playeronefee", "10000000.0000 ENU"]' -p playeroneiss
# enucli push action playeroneiss issue '[ "playeronefee", "100000.0000 ENU"]' -p playeronefee
# enucli push action playeroneiss transfer '["playeronefee", "oneplayerone", "10.1111 ENU", "testuseraaaa"]' -p playeronefee

# follow test must throw
# enucli push action playeronefee create '[ "playeronefee", "10000000.0000 ENU"]' -p playeronefee
# enucli push action playeronefee create '[ "playeronefee", "10000000.0000 CGT"]' -p playeronefee
# enucli push action playeronefee issue '[ "playeronefee", "10000000.0000 ENU", "initial supply" ]' -p playeronefee
# enucli push action playeronefee issue '[ "playeronefee", "10000000.0000 CGT", "initial supply" ]' -p playeronefee
# enucli push action playeronefee transfer '["playeronefee", "oneplayerone", "10.1111 ENU", "testuseraaaa"]' -p playeronefee
# enucli push action playeronefee transfer '["playeronefee", "oneplayerone", "10.1111 CGT", "testuseraaaa"]' -p playeronefee
