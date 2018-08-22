#! /bin/bash

enucli wallet import --private-key 5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3

# assume you have imported the testers' keys
enucli wallet import --private-key 5KKUS3oXbTQpmL1TqYm8xo8Zy9XNQ6DwwJwuUzAzo2CBRqSRweV
enucli wallet import --private-key 5JVVQpGNWLz3nUcPDjeixSH6mi2pRW6da9ErLWK3RaMipWJox37
enucli wallet import --private-key 5K9ZQFXzeesmjdUqPQytg1KKiDGTWSDsW5jQHqUZttVi9VLwrJX
enucli wallet import --private-key 5JcxRgm2WkD5x482BG3gjE4kJde9CqMutPcq3k2UYYajuNkR5wN
enucli wallet import --private-key 5JrsSNfzM5YDQmdkYfgRBQtSQ9WZccM6n7CRGrBE9g7CrG6NHhM
enucli wallet import --private-key 5JYAkZt781wVvtxg3Y41hBRT2SSxEVeNTiyJKKrBXTMGCwKQJCx
enucli wallet import --private-key 5JSHN5z385ZEikDzdrU1ixZp1XYoNw7m8X9s1LYaUWxt3Y3gbDG
enucli wallet import --private-key 5KGx9WDVGWeXU7twpQ9dRsJUEsk4tf7b4UcLEYT8JANJyeiUsyD
enucli wallet import --private-key 5KCa3Pvq5APMZYCz3qABisnDA1sUtYiKwinLWU7aHqv2kU7BQYb
enucli wallet import --private-key 5KA7efGfZdFpgEX37uPiDQAarfacs5X5W6GUzk9kdQzhrSZJiW9
enucli wallet import --private-key 5JTGHQaHSXz6HCqt4QCtabfptz7pCrpmMMLdZD7nbUSRtEvEcuL


# enu.token = ENU87infeXNHe3cRzc71MNcQESM6DFWYjMU5MvSSrji7nup4x4K2E=KEY:5KKUS3oXbTQpmL1TqYm8xo8Zy9XNQ6DwwJwuUzAzo2CBRqSRweV
enucli create account enumivo enu.token ENU87infeXNHe3cRzc71MNcQESM6DFWYjMU5MvSSrji7nup4x4K2E ENU87infeXNHe3cRzc71MNcQESM6DFWYjMU5MvSSrji7nup4x4K2E
enucli set contract enu.token ./enu.token -p enu.token

enucli push action enu.token create '[ "enu.token", "1000000000.0000 ENU"]' -p enu.token
enucli push action enu.token issue '[ "enu.token", "1000000000.0000 ENU", "initial supply" ]' -p enu.token


# testuseraaaa = ENU7aozLD8SsjCtxhMTfDaCh6PBrQKwJ1NvSDCtxe4SvkmzkmLudm=KEY:5JVVQpGNWLz3nUcPDjeixSH6mi2pRW6da9ErLWK3RaMipWJox37
enucli create account enumivo testuseraaaa ENU7aozLD8SsjCtxhMTfDaCh6PBrQKwJ1NvSDCtxe4SvkmzkmLudm ENU7aozLD8SsjCtxhMTfDaCh6PBrQKwJ1NvSDCtxe4SvkmzkmLudm
# testuseraaab = ENU6dd5jBWi32fg12NyP8sX6fdWU58xo7dDcdPB5wxJWfBiomP3eo=KEY:5K9ZQFXzeesmjdUqPQytg1KKiDGTWSDsW5jQHqUZttVi9VLwrJX
enucli create account enumivo testuseraaab ENU6dd5jBWi32fg12NyP8sX6fdWU58xo7dDcdPB5wxJWfBiomP3eo ENU6dd5jBWi32fg12NyP8sX6fdWU58xo7dDcdPB5wxJWfBiomP3eo
# testuseraaac = ENU6Bwd1ydLTXfjHsFGFVvwkhASezVzENXNJBx5MyJXsPiEsMrWFW=KEY:5JcxRgm2WkD5x482BG3gjE4kJde9CqMutPcq3k2UYYajuNkR5wN
enucli create account enumivo testuseraaac ENU6Bwd1ydLTXfjHsFGFVvwkhASezVzENXNJBx5MyJXsPiEsMrWFW ENU6Bwd1ydLTXfjHsFGFVvwkhASezVzENXNJBx5MyJXsPiEsMrWFW
# testuseraaad = ENU7XEHhumorTShir1ks2dq6XB1RAWtKk7sSGhNzUBSJx6Xn9Q3QU=KEY:5JrsSNfzM5YDQmdkYfgRBQtSQ9WZccM6n7CRGrBE9g7CrG6NHhM
enucli create account enumivo testuseraaad ENU7XEHhumorTShir1ks2dq6XB1RAWtKk7sSGhNzUBSJx6Xn9Q3QU ENU7XEHhumorTShir1ks2dq6XB1RAWtKk7sSGhNzUBSJx6Xn9Q3QU
# testuseraaae = ENU8j55oVcKj1cNnY9UFo1f4hDnt9XrKTpNasmJQAgZBPdR55JZ3n=KEY:5JYAkZt781wVvtxg3Y41hBRT2SSxEVeNTiyJKKrBXTMGCwKQJCx
enucli create account enumivo testuseraaae ENU8j55oVcKj1cNnY9UFo1f4hDnt9XrKTpNasmJQAgZBPdR55JZ3n ENU8j55oVcKj1cNnY9UFo1f4hDnt9XrKTpNasmJQAgZBPdR55JZ3n
# testuseraaaf = ENU696FezUddFSEBzqTq93eiCJPZthdQhxz6gkBxKgx51D9ZrFR6a=KEY:5JSHN5z385ZEikDzdrU1ixZp1XYoNw7m8X9s1LYaUWxt3Y3gbDG
enucli create account enumivo testuseraaaf ENU696FezUddFSEBzqTq93eiCJPZthdQhxz6gkBxKgx51D9ZrFR6a ENU696FezUddFSEBzqTq93eiCJPZthdQhxz6gkBxKgx51D9ZrFR6a
# testuseraaag = ENU5hvH6RTo4KcRfdBGnhm42pLBWCPTm7rivknWCXUFaBkQBa8ZQg=KEY:5KGx9WDVGWeXU7twpQ9dRsJUEsk4tf7b4UcLEYT8JANJyeiUsyD
enucli create account enumivo testuseraaag ENU5hvH6RTo4KcRfdBGnhm42pLBWCPTm7rivknWCXUFaBkQBa8ZQg ENU5hvH6RTo4KcRfdBGnhm42pLBWCPTm7rivknWCXUFaBkQBa8ZQg
# testuseraaah = ENU5wsrYqCrXDZrdWShEgeF43riaf9NEdJTdfoB1RhEBspDFzG3ZH=KEY:5KCa3Pvq5APMZYCz3qABisnDA1sUtYiKwinLWU7aHqv2kU7BQYb
enucli create account enumivo testuseraaah ENU5wsrYqCrXDZrdWShEgeF43riaf9NEdJTdfoB1RhEBspDFzG3ZH ENU5wsrYqCrXDZrdWShEgeF43riaf9NEdJTdfoB1RhEBspDFzG3ZH
# testuseraaai = ENU6tVvMqXjHKCV37kPdJeDu49UmBgQKPv12ndZToPBL5N4iMxjVQ=KEY:5KA7efGfZdFpgEX37uPiDQAarfacs5X5W6GUzk9kdQzhrSZJiW9
enucli create account enumivo testuseraaai ENU6tVvMqXjHKCV37kPdJeDu49UmBgQKPv12ndZToPBL5N4iMxjVQ ENU6tVvMqXjHKCV37kPdJeDu49UmBgQKPv12ndZToPBL5N4iMxjVQ
# testuseraaaj = ENU5JCzKF1NHnY2Z4EwxG3EjFvoKCukjjzmB1moAU61eqRucGdeqd=KEY:5JTGHQaHSXz6HCqt4QCtabfptz7pCrpmMMLdZD7nbUSRtEvEcuL
enucli create account enumivo testuseraaaj ENU5JCzKF1NHnY2Z4EwxG3EjFvoKCukjjzmB1moAU61eqRucGdeqd ENU5JCzKF1NHnY2Z4EwxG3EjFvoKCukjjzmB1moAU61eqRucGdeqd

enucli transfer enu.token testuseraaaa "100000 ENU" "test" -p enu.token
enucli transfer enu.token testuseraaab "100000 ENU" "test" -p enu.token
enucli transfer enu.token testuseraaac "100000 ENU" "test" -p enu.token
enucli transfer enu.token testuseraaad "100000 ENU" "test" -p enu.token
enucli transfer enu.token testuseraaae "100000 ENU" "test" -p enu.token
enucli transfer enu.token testuseraaaf "100000 ENU" "test" -p enu.token
enucli transfer enu.token testuseraaag "100000 ENU" "test" -p enu.token
enucli transfer enu.token testuseraaah "100000 ENU" "test" -p enu.token
enucli transfer enu.token testuseraaai "100000 ENU" "test" -p enu.token
enucli transfer enu.token testuseraaaj "100000 ENU" "test" -p enu.token

