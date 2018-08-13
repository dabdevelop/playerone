const ecc = require('eosjs-ecc')

function generateSignatureProvider(amount){
    for(var i = 0; i < amount; i++){
        ecc.randomKey().then(privateKey => {
            // signature-provider = EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV=KEY:5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3
            console.log('signature-provider =', ecc.privateToPublic(privateKey)+'=KEY:'+privateKey);
        })
    }    
}


function generateTesters(amount){
    var j = 0x61;
    // for(var i = 0; i < amount; i++){
    //     ecc.randomKey().then(privateKey=>{
    //         console.log('# testuser' + ('aaa' + String.fromCharCode(j)),'=', ecc.privateToPublic(privateKey) + '=KEY:' + privateKey);
    //         console.log('cleos create account eosio', 'testuser' + ('aaa' + String.fromCharCode(j)), ecc.privateToPublic(privateKey), ecc.privateToPublic(privateKey));
    //         j++;
    //     });
    // }
    
    
    for(var i = 0; i < amount; i++){
        // cleos transfer eosio.token testuser1111  "10000 EOS" "test" -p eosio.token
        console.log('cleos transfer eosio.token testuser'+ ('aaa' + String.fromCharCode(0x61 + i)) +' "10000 EOS" "test" -p eosio.token');       
    }
}

function generateOfficialAccount(names){
    // testers = [name1, name2]
    var i = 0;
    for(var n in names){
        ecc.randomKey().then(privateKey=>{
            console.log('# ' + names[i],'=', ecc.privateToPublic(privateKey) + '=KEY:' + privateKey);
            console.log('cleos create account eosio', names[i], ecc.privateToPublic(privateKey), ecc.privateToPublic(privateKey));
            // console.log('cleos transfer eosio.token '+ names[i] +' "10000 EOS" "test" -p eosio.token');       
            i++;
        });
    }
}

// generateTesters(10);
generateOfficialAccount(['oneplayerone', 'blackholeeos', 'playeronefee', 'playeroneiss'])