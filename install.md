## Player One Documentaion

### Pre-installation

> Assume you have pre-installed eos https://github.com/EOSIO/eos

> Assume you have pre-installed eosio.wasmsdk https://github.com/EOSIO/eosio.wasmsdk

> Assume you have pre-installed eosio.contracts https://github.com/EOSIO/eosio.contracts

### Configuration

#### Copy the eos configure file `config.ini` to the eos config folder

`nodeos` uses a custom configuration folder. The location of this folder is determined by your system.

> Mac OS: ~/Library/Application\ Support/eosio/nodeos/config/

> Linux: ~/.local/share/eosio/nodeos/config/

For example, on Mac:

```bash
    cp ./config.ini ~/Library/Application\ Support/eosio/nodeos/config/
```

Then maybe you need change this two dir path in `eos_init.sh`
```bash
    EOS_HOME=../eos
    EOS_CONTRACT_HOME=../eosio.contracts
```

#### You need to import the eosio master key to get root privilege in eos

Before run it you may need to unlock eos wallet.

```bash
    cleos wallet unlock
```
You can find the config infomation in eos config file: `config.ini`.

```config
    signature-provider = EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV=KEY:5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3
```

Just import the key:
```bash
    cleos wallet import --private-key 5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3
```

#### Run local test net and test

##### First run the eos local test net

Run `nodeos` in your `eos build folder`:

```bash
    ./programs/nodeos/nodeos
```
If you need to clear the blockchain data, just remove the eos block data folder.

For example, on Mac:
```bash
    rm -rf ~/Library/Application\ Support/eosio/nodeos/data/*
```

##### Then run the all in one script

Before run it you may need to unlock eos wallet.

```bash
    cleos wallet unlock
```

Then run. Enjoy!

```bash
    ./run.sh
```





