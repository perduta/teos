#include "eosc_get_commands.hpp"

const char* getSubcommands = R"EOF(
ERROR: RequiredError: Subcommand required
Retrieve various items and information from the blockchain
Usage : ./ eosc [OPTIONS] get SUBCOMMAND [OPTIONS]

Subcommands:
    info            Get current blockchain information
    block           Retrieve a full block from the blockchain
    account         Retrieve an account from the blockchain
    code            Retrieve the code and ABI for an account
    table           Retrieve the contents of a database table
    accounts        Retrieve accounts associated with a public key
    servants        Retrieve accounts which are servants of a given account
    transaction     Retrieve a transaction from the blockchain
    transactions    Retrieve all transactions with specific account name referenced in their scope
)EOF";

const std::string getCommandPath = "/v1/chain/";

const char* setSubcommands = R"EOF(
ERROR: RequiredError: Subcommand required
Set or update blockchain state
Usage: ./eosc [OPTIONS] set SUBCOMMAND [OPTIONS]

Subcommands:
    contract        Create or update the contract on an account
    producer        Approve/unapprove producer
    proxy           Set proxy account for voting
    account         set or update blockchain account state
    action          set or update blockchain action state
)EOF";

const char* benchmarkSubcommands = R"EOF(
ERROR: RequiredError: Subcommand required
Configure and execute benchmarks
Usage: ./eosc [OPTIONS] benchmark SUBCOMMAND [OPTIONS]

Subcommands:
    setup           Configures initial condition for benchmark
    transfer        executes random transfers among accounts
)EOF";

const char* pushSubcommands = R"EOF(
ERROR: RequiredError: Subcommand required
Push arbitrary transactions to the blockchain
Usage: ./eosc [OPTIONS] push SUBCOMMAND [OPTIONS]

Subcommands:
    message         Push a transaction with a single message
    transaction     Push an arbitrary JSON transaction
    transactions    Push an array of arbitrary JSON transactions
)EOF";