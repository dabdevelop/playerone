!#bash
# signature-provider = EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV=KEY:5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3
# cleos wallet import -h --private-key 5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3

EOS_HOME=/Users/taofeng/Github/eos
EOS_CONTRACT_HOME=/Users/taofeng/Github/eosio.contracts

cleos set contract eosio ${EOS_HOME}/build/contracts/eosio.bios -p eosio@active

cleos create account eosio eosio.msig EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos set contract eosio.msig ${EOS_CONTRACT_HOME}/build/eosio.msig -p eosio.msig@active

cleos create account eosio eosio.system EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos set contract eosio.system ${EOS_CONTRACT_HOME}/build/eosio.system -p eosio.system@active

cleos create account eosio eosio.sudo EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos set contract eosio.sudo ${EOS_CONTRACT_HOME}/build/eosio.sudo -p eosio.sudo@active

# eosio.token = EOS87infeXNHe3cRzc71MNcQESM6DFWYjMU5MvSSrji7nup4x4K2E=KEY:5KKUS3oXbTQpmL1TqYm8xo8Zy9XNQ6DwwJwuUzAzo2CBRqSRweV
cleos create account eosio eosio.token EOS87infeXNHe3cRzc71MNcQESM6DFWYjMU5MvSSrji7nup4x4K2E EOS87infeXNHe3cRzc71MNcQESM6DFWYjMU5MvSSrji7nup4x4K2E
cleos set contract eosio.token ./build/eosio.token -p eosio.token

cleos push action eosio.token create '[ "eosio.token", "1000000000.0000 EOS"]' -p eosio.token
cleos push action eosio.token issue '[ "eosio.token", "1000000000.0000 EOS", "initial supply" ]' -p eosio.token

# testuseraaaa = EOS7aozLD8SsjCtxhMTfDaCh6PBrQKwJ1NvSDCtxe4SvkmzkmLudm=KEY:5JVVQpGNWLz3nUcPDjeixSH6mi2pRW6da9ErLWK3RaMipWJox37
cleos create account eosio testuseraaaa EOS7aozLD8SsjCtxhMTfDaCh6PBrQKwJ1NvSDCtxe4SvkmzkmLudm EOS7aozLD8SsjCtxhMTfDaCh6PBrQKwJ1NvSDCtxe4SvkmzkmLudm
# testuseraaab = EOS6dd5jBWi32fg12NyP8sX6fdWU58xo7dDcdPB5wxJWfBiomP3eo=KEY:5K9ZQFXzeesmjdUqPQytg1KKiDGTWSDsW5jQHqUZttVi9VLwrJX
cleos create account eosio testuseraaab EOS6dd5jBWi32fg12NyP8sX6fdWU58xo7dDcdPB5wxJWfBiomP3eo EOS6dd5jBWi32fg12NyP8sX6fdWU58xo7dDcdPB5wxJWfBiomP3eo
# testuseraaac = EOS6Bwd1ydLTXfjHsFGFVvwkhASezVzENXNJBx5MyJXsPiEsMrWFW=KEY:5JcxRgm2WkD5x482BG3gjE4kJde9CqMutPcq3k2UYYajuNkR5wN
cleos create account eosio testuseraaac EOS6Bwd1ydLTXfjHsFGFVvwkhASezVzENXNJBx5MyJXsPiEsMrWFW EOS6Bwd1ydLTXfjHsFGFVvwkhASezVzENXNJBx5MyJXsPiEsMrWFW
# testuseraaad = EOS7XEHhumorTShir1ks2dq6XB1RAWtKk7sSGhNzUBSJx6Xn9Q3QU=KEY:5JrsSNfzM5YDQmdkYfgRBQtSQ9WZccM6n7CRGrBE9g7CrG6NHhM
cleos create account eosio testuseraaad EOS7XEHhumorTShir1ks2dq6XB1RAWtKk7sSGhNzUBSJx6Xn9Q3QU EOS7XEHhumorTShir1ks2dq6XB1RAWtKk7sSGhNzUBSJx6Xn9Q3QU
# testuseraaae = EOS8j55oVcKj1cNnY9UFo1f4hDnt9XrKTpNasmJQAgZBPdR55JZ3n=KEY:5JYAkZt781wVvtxg3Y41hBRT2SSxEVeNTiyJKKrBXTMGCwKQJCx
cleos create account eosio testuseraaae EOS8j55oVcKj1cNnY9UFo1f4hDnt9XrKTpNasmJQAgZBPdR55JZ3n EOS8j55oVcKj1cNnY9UFo1f4hDnt9XrKTpNasmJQAgZBPdR55JZ3n
# testuseraaaf = EOS696FezUddFSEBzqTq93eiCJPZthdQhxz6gkBxKgx51D9ZrFR6a=KEY:5JSHN5z385ZEikDzdrU1ixZp1XYoNw7m8X9s1LYaUWxt3Y3gbDG
cleos create account eosio testuseraaaf EOS696FezUddFSEBzqTq93eiCJPZthdQhxz6gkBxKgx51D9ZrFR6a EOS696FezUddFSEBzqTq93eiCJPZthdQhxz6gkBxKgx51D9ZrFR6a
# testuseraaag = EOS5hvH6RTo4KcRfdBGnhm42pLBWCPTm7rivknWCXUFaBkQBa8ZQg=KEY:5KGx9WDVGWeXU7twpQ9dRsJUEsk4tf7b4UcLEYT8JANJyeiUsyD
cleos create account eosio testuseraaag EOS5hvH6RTo4KcRfdBGnhm42pLBWCPTm7rivknWCXUFaBkQBa8ZQg EOS5hvH6RTo4KcRfdBGnhm42pLBWCPTm7rivknWCXUFaBkQBa8ZQg
# testuseraaah = EOS5wsrYqCrXDZrdWShEgeF43riaf9NEdJTdfoB1RhEBspDFzG3ZH=KEY:5KCa3Pvq5APMZYCz3qABisnDA1sUtYiKwinLWU7aHqv2kU7BQYb
cleos create account eosio testuseraaah EOS5wsrYqCrXDZrdWShEgeF43riaf9NEdJTdfoB1RhEBspDFzG3ZH EOS5wsrYqCrXDZrdWShEgeF43riaf9NEdJTdfoB1RhEBspDFzG3ZH
# testuseraaai = EOS6tVvMqXjHKCV37kPdJeDu49UmBgQKPv12ndZToPBL5N4iMxjVQ=KEY:5KA7efGfZdFpgEX37uPiDQAarfacs5X5W6GUzk9kdQzhrSZJiW9
cleos create account eosio testuseraaai EOS6tVvMqXjHKCV37kPdJeDu49UmBgQKPv12ndZToPBL5N4iMxjVQ EOS6tVvMqXjHKCV37kPdJeDu49UmBgQKPv12ndZToPBL5N4iMxjVQ
# testuseraaaj = EOS5JCzKF1NHnY2Z4EwxG3EjFvoKCukjjzmB1moAU61eqRucGdeqd=KEY:5JTGHQaHSXz6HCqt4QCtabfptz7pCrpmMMLdZD7nbUSRtEvEcuL
cleos create account eosio testuseraaaj EOS5JCzKF1NHnY2Z4EwxG3EjFvoKCukjjzmB1moAU61eqRucGdeqd EOS5JCzKF1NHnY2Z4EwxG3EjFvoKCukjjzmB1moAU61eqRucGdeqd

cleos transfer eosio.token testuseraaaa "10000 EOS" "test" -p eosio.token
cleos transfer eosio.token testuseraaab "10000 EOS" "test" -p eosio.token
cleos transfer eosio.token testuseraaac "10000 EOS" "test" -p eosio.token
cleos transfer eosio.token testuseraaad "10000 EOS" "test" -p eosio.token
cleos transfer eosio.token testuseraaae "10000 EOS" "test" -p eosio.token
cleos transfer eosio.token testuseraaaf "10000 EOS" "test" -p eosio.token
cleos transfer eosio.token testuseraaag "10000 EOS" "test" -p eosio.token
cleos transfer eosio.token testuseraaah "10000 EOS" "test" -p eosio.token
cleos transfer eosio.token testuseraaai "10000 EOS" "test" -p eosio.token
cleos transfer eosio.token testuseraaaj "10000 EOS" "test" -p eosio.token

