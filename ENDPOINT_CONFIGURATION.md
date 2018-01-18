block-log-dir = blocks
readonly = 0
send-whole-blocks = true
shared-file-dir = blockchain
shared-file-size = 8192
http-server-address = [eos-testapi.tokenika.io:8888](http://eos-testapi.tokenika.io:8888/)
p2p-listen-endpoint = [eos-testp2p.tokenika.io:9876](http://eos-testp2p.tokenika.io:9876/)
p2p-server-address = [eos-testp2p.tokenika.io:9876](http://eos-testp2p.tokenika.io:9876/)
agent-name = "Tokenika EOS Test Node"
allowed-connection = any
p2p-peer-address = [p2p-testnet1.eos.io:9876](http://p2p-testnet1.eos.io:9876/)
plugin = eosio::chain_api_plugin
plugin = eosio::account_history_plugin
plugin = eosio::account_history_api_plugin
***
WARNING, non-local wallet api
Do not use it unless you know what are you doing!
***
plugin = eosio::wallet_api_plugin
plugin = eosio::wallet_plugin
block-interval-seconds = 2
pending-txn-depth-limit = 60
sync-fetch-span = 100