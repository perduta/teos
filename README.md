# Tokenika's alternative for EOS command line interface

## Rationale

For those less familiar with EOS codebase, `eosc` is the official CLI (Command Line Interface) for EOS. It interacts with the EOS blockchain by connecting to a full node called `eosd`, which can run either locally or on a remote server.

When working with EOS smart-contracts, we've found that `eosc` has some inconvenient limitations:

* Firstly, it's hard to use `eosc` programmatically, as it doesn't offer an API.
* Secondly, it is quite heavyweight in terms of external dependencies, as it's tightly connected to the entire EOS codebase.
* And finally, it is not ready to be used in Windows environment, while our plans include opening up EOS for smart-contract development on Windows.

It could be enough for us to develop a minimal C++ library acting as an EOS API and this way implement all the commands supported by `eosc`. However, it was a short step to provide such a library with a command line interface, and thus create a full-blown `eosc` replacement, which we've named `teos`. 

Here are the benefits of using `teos` instead of `eosc`:

* With `teos` you can do everything available in `eosc` and much more, as we've added a richer & more useful command option list.
* Also, as `teos` is not dependent on the entire EOS codebase, it can be easily compiled on any platform, including Windows, which is not the case with `eosc`.
* And last but not least, `teos` has an underlying library which offers a proper API which you can use to interact programmatically with `eosd` (EOS full node).

For obvious reasons everything we do is open source. The source code of `teos` is located in [this repository](https://github.com/tokenika/teos).

Note: to make our project fully cross-platform (including Windows), we needed to clone and modify some of the support libraries from the EOS codebase. You'll find more details about it in [this document](https://github.com/tokenika/teos/blob/master/EOS_LIBS_AMENDMENTS.md).

## Comparison

As it was mentioned above, `teos` covers the same functionality as `eosc`, but it's more user friendly and offers a wider selection of options.

Let's compare the `get block` command options and the response `eosc` gives:
```
./eosc get block --help
```
```
ERROR: RequiredError: block
Retrieve a full block from the blockchain
Usage: ./eosc get block block
Positionals:
block TEXT                  The number or ID of the block to retrieve
```
Whereas a similar command in `teos` will produce something like this:

```
./teos get block --help
```

```
Retrieve a full block from the blockchain
Usage: ./teos get block [block_num_or_id][options]
Usage: ./teos get block [-j {"block_num_or_id":*}][options]

Options:
  -h [ --help ]           Help screen
  -n [ --block_num ] arg  Block number
  -i [ --block_id ] arg   Block id
  -j [ --json ] arg       Json argument
  -v [ --verbose ]        Print the entire received json
  -r [ --raw ]            Raw print
  -e [ --example ]        Usage example
```
And now let's compare the `get block` command usage and the response `eosc` gives:
```
./eosc get block 25
```
```
{
  "previous": "00000018b5e0ffcd3dfede45bc261e3a04de9f1f40386a69821780e063a41448",
  "timestamp": "2017-11-29T09:50:03",
  "transaction_merkle_root": "0000000000000000000000000000000000000000000000000000000000000000",
  "producer": "initf",
  "producer_changes": [],
  "producer_signature": "2005db1a193cc3597fdc3bd38a4375df2a9f9593390f9431f7a9b53701cd46a1b5418b9cd68edbdf2127d6ececc4d66b7a190e72a97ce9adfcc750ef0a770f5619",
  "cycles": [],
  "id": "000000190857c9fb43d62525bd29dc321003789c075de593ce7224bde7fc2284",
  "block_num": 25,
  "refBlockPrefix": 623236675
}
```
Whereas a similar command in `teos` will produce a response which is less verbose by default, thus more readable:

```
./teos get block 25
```

```
## block number: 25
## timestamp: 2017-11-29T09:50:03
## ref block prefix: 623236675
```
But you can make it verbose, if you need:
```
./teos get block 25 --verbose
```
```
{
    "previous": "00000018b5e0ffcd3dfede45bc261e3a04de9f1f40386a69821780e063a41448",
    "timestamp": "2017-11-29T09:50:03",
    "transaction_merkle_root": "0000000000000000000000000000000000000000000000000000000000000000",
    "producer": "initf",
    "producer_changes": "",
    "producer_signature": "2005db1a193cc3597fdc3bd38a4375df2a9f9593390f9431f7a9b53701cd46a1b5418b9cd68edbdf2127d6ececc4d66b7a190e72a97ce9adfcc750ef0a770f5619",
    "cycles": "",
    "id": "000000190857c9fb43d62525bd29dc321003789c075de593ce7224bde7fc2284",
    "block_num": "25",
    "refBlockPrefix": "623236675"
}
```
Furthermore, you can make it both verbose and unformatted:
```
./teos get block 25 --verbose --raw
```
```
{"previous":"00000018b5e0ffcd3dfede45bc261e3a04de9f1f40386a69821780e063a41448","timestamp":"2017-11-29T09:50:03","transaction_merkle_root":"0000000000000000000000000000000000000000000000000000000000000000","producer":"initf","producer_changes":"","producer_signature":"2005db1a193cc3597fdc3bd38a4375df2a9f9593390f9431f7a9b53701cd46a1b5418b9cd68edbdf2127d6ececc4d66b7a190e72a97ce9adfcc750ef0a770f5619","cycles":"","id":"000000190857c9fb43d62525bd29dc321003789c075de593ce7224bde7fc2284","block_num":"25","refBlockPrefix":"623236675"}
```
Also, you can supply the arguments in *json* format:
```
./teos get block --json '{"block_num_or_id":"56"}'
```
```
##         block number: 56
##            timestamp: 2017-11-29T10:02:18
##     ref block prefix: 273573026
```
And finally, for each command you can invoke an example showcasing its usage:
```
./teos get block --example
```
```
// Invoke 'GetInfo' command:
    ptree getInfoJson;
    GetInfo getInfo(getInfoPostJson);
    cout << getInfo.toStringRcv() << endl;

/*
printout:

{
    "server_version": "9703495c",
    "head_block_num": "1707240",
    "last_irreversible_block_num": "1707225",
    "head_block_id": "001a0ce87ca6e2d0fc19b8a02e9241c658bea0365f4e6f035ce6602db04611bd",
    "head_block_time": "2017-12-25T14:11:31",
    "head_block_producer": "inito",
    "recent_slots": "1111111111111111111111111111111111111111111111111111111111111111",
    "participation_rate": "1.00000000000000000"
}

*/

// Use reference to the last block:
    ptree GetBlockJson;
    GetBlock_poGetBlockJsont_json.put("block_num_or_id",
      getInfo.get<int>("last_irreversible_block_num"));
    GetBlock GetBlock(GetBlock_post_json);
    cout << GetBlock.toStringRcv() << endl;

/*
printout:

{
    "previous": "001a0cd8422216f2828ef5056e9371439f80665cee99d72a5f3162ae7c0495fd",
    "timestamp": "2017-12-25T14:11:16",
    "transaction_merkle_root": "0000000000000000000000000000000000000000000000000000000000000000",
    "producer": "initn",
    "producer_changes": "",
    "producer_signature": "1f382b4fe716f683c8a7ebd15fe5f5266c75a24f75b9b212fc3cc3f7db11f5258b08e5aebc7680784c240e0f8d0ea7540dfb4ab8dcbe5cd8b492876e8f59bb4ea8",
    "cycles": "",
    "id": "001a0cd98eb6f7e0f8e7803b098082b35f1348672561af193ead3d1b1a281bcf",
    "block_num": "1707225",
    "ref_block_prefix": "998303736"
}

*/
```

## Using `teos` as EOS API

In our view, the real value of our efforts is actually the library that's behind `teos`. As we mentioned before, the `teosLib` library acts as a full-blown API for EOS.

Let's consider a code snippet illustrating its usage:

```
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "teosLib/teos_get_commands.hpp"

int main(int argc, char *argv[])
{
  using namespace tokenika::teos;

  TeosCommand::host = "198.100.148.136";
  TeosCommand::port = "8888";

  ptree getInfoJson;

  // Invoke 'GetInfo' command:
  GetInfo getInfo(getInfoJson);
  cout << getInfo.toStringRcv() << endl;

  if (getInfo.isError()) {
    return -1;
  }

  ptree getBlockJson;

  // Use reference to the last block:
  getBlockJson.put("block_num_or_id",
    getInfo.get<int>("last_irreversible_block_num"));
  GetBlock getBlock(getBlockJson);
  cout << getBlock.toStringRcv() << endl;

  if (getBlock.isError()) {
    return -1;
  }

  return 0;
}

```

Here is the outcome of the above code:

```
{
    "server_version": "9703495c",
    "head_block_num": "1707240",
    "last_irreversible_block_num": "1707225",
    "head_block_id": "001a0ce87ca6e2d0fc19b8a02e9241c658bea0365f4e6f035ce6602db04611bd",
    "head_block_time": "2017-12-25T14:11:31",
    "head_block_producer": "inito",
    "recent_slots": "1111111111111111111111111111111111111111111111111111111111111111",
    "participation_rate": "1.00000000000000000"
}

{
    "previous": "001a0cd8422216f2828ef5056e9371439f80665cee99d72a5f3162ae7c0495fd",
    "timestamp": "2017-12-25T14:11:16",
    "transaction_merkle_root": "0000000000000000000000000000000000000000000000000000000000000000",
    "producer": "initn",
    "producer_changes": "",
    "producer_signature": "1f382b4fe716f683c8a7ebd15fe5f5266c75a24f75b9b212fc3cc3f7db11f5258b08e5aebc7680784c240e0f8d0ea7540dfb4ab8dcbe5cd8b492876e8f59bb4ea8",
    "cycles": "",
    "id": "001a0cd98eb6f7e0f8e7803b098082b35f1348672561af193ead3d1b1a281bcf",
    "block_num": "1707225",
    "ref_block_prefix": "998303736"
}

```

## List of currently supported commands

At this very initial stage of our project, we haven't ported all the commands available in `eosc`. Below is the list of commands `teos` supports in this release:

```
version client
get info
get block
get account
get code
get table
wallet create
wallet list
wallet keys
wallet import
wallet open
wallet lock
wallet lock all
wallet unlock
create key
```

## Building on Ubuntu

#### Dependencies

If you are on Ubuntu, the only thing you need to do is have the EOS codebase complied and installed. Just follow the instructions listed in [the official EOS repository](https://github.com/EOSIO/eos).

We don't really need any of the code contained there, however we recommend to make use of the fact that EOS automated build script sorts out all the external dependencies (i.e.  [Boost](http://www.boost.org/), [OpenSSL](https://www.openssl.org/), [GMP](https://gmplib.org/) & [Secp256k1](https://github.com/bitcoin-core/secp256k1)) that `teos` requires. And, unless you want to use our remote server for this purpose, you will need a locally running EOS full node anyway to play with `teos`, so no effort is actually wasted.

What you will also need is `cmake` version 3.8 or higher. You can verify it using this command:

```
cmake --version
```

If your `cmake` is lower than 3.8, run this command to remove it:

```
sudo apt-get purge cmake
```

And then run these commands to build it from the source code:

```
version=3.8
build=0
mkdir ~/temp
cd ~/temp
wget https://cmake.org/files/v$version/cmake-$version.$build.tar.gz
tar -xzvf cmake-$version.$build.tar.gz
cd cmake-$version.$build/
./bootstrap
make -j4
sudo make install
```

When the build process is complete, open a new terminal window and make sure `cmake` is actually version 3.8: 

```
cmake --version
```

#### Cloning the source code

Navigate to a location of your choice on your machine and clone *teos* repository:

```
git clone https://github.com/tokenika/teos.git
```
#### Compilation

Navigate to the `teos/teos` folder and create a new folder named `build`:

```
cd teos/teos
mkdir build
cd build
```
Run CMake:
```
cmake ..
```
Make sure there are no errors, and then proceed with the actual compilation:
```
make install
```
As the result of the compilation, you should be able to find those two files in the `install` folder:
* `lib/libteoslib.a` is a static library acting as an API for EOS
* `bin/teos` is the CLI executable making use of the above library

#### Testing on remote sever

Open a terminal window, navigate to the `install/bin` folder and run `teos`:
```
./teos 198.100.148.136:8888 get info
```
The above command will connect to EOS full node running on one of our testnet servers.

Alternatively, you can use the predefined placeholder `tokenika` instead of  `198.100.148.136:8888`:

```
./teos tokenika get info
```

You can try simple queries, e.g.

```
./teos tokenika get block 25
```

```
##         block number: 25
##            timestamp: 2017-12-05T19:55:56
##     ref block prefix: 1139663381
```

Or you can test private key generation:

```
./teos tokenika create key
```

```
##             key name: default
##          private key: 5JyL28JPQbPTYwTpjKRcXvfj6nwUKgCmHnJaD28nmmWMpHXukVn
##           public key: EOS7TxBhoCwAWXoV8uhtgjz4inTLwiwcySvrVhGNYcjhw75wFJ9uA
```

#### Testing on localhost

If you have complied the entire EOS codebase and have `eosd` running on your local machine, you can also test `teos` locally:
```
./teos localhost get info
```

Or just:

```
./teos get info
```

## Building on Windows

#### Prerequisites

We assume that your computer is 64bit.

We also assume that you have [Git 2.15.1](https://git-scm.com/downloads), [CMake 3.10.1](https://cmake.org/download/) and [MS Visual Studio 2017](https://www.visualstudio.com/downloads/) installed. If not, please follow instructions available in the the above links.

As far as Visual Studio is concerned, you will only need the most basic module called *Universal Windows Platform Development* (and from optional tools *C++ Universal Windows Platform Tools*).

#### Dependencies

For most of its functionality `teos` is only dependent on [Boost](http://www.boost.org/). However, because it also needs to able to generate private keys, additionally `teos` is dependent on [OpenSSL](https://www.openssl.org/), [GMP](https://gmplib.org/) and [Secp256k1](https://github.com/bitcoin-core/secp256k1), as specified in EOS documentation.

On Windows the main difficulty is to have all those dependencies as Windows-compiled libraries. Advanced Windows users might want to build everything from source files (it's certainly doable) and ultimately we will aim for that. However, at this stage we recommend using pre-compiled binaries:

- Boost version 1.64 (not higher as it might be incompatible with CMake)
  - Windows binaries are available [here](https://sourceforge.net/projects/boost/files/boost-binaries/1.64.0/) (for a 64bit machine select `boost_1_64_0-msvc-14.1-64.exe`).
  - Define an environment variable `BOOST_INCLUDEDIR` pointing to the location you've chosen to store the Boost libraries (e.g. `C:\Local\boost_1_64_0`).
  - Define an environment variable `BOOST_LIBRARYDIR` pointing to the `lib64-msvc-14.1` folder inside the location you've chosen to store the Boost libraries (e.g. `C:\Local\boost_1_64_0\lib64-msvc-14.1`).
- OpenSLL version 1.1.0
  - Windows binaries are available [here](https://slproweb.com/products/Win32OpenSSL.html) (for a 64bit machine select `Win64OpenSSL-1_1_0g.exe`).
  - Run the installer and when prompted choose to copy the DLLs to the `bin` directory.
  - Define an environment variable `OPENSSL_ROOT_DIR` pointing to the location you've chosen to store the OpenSLL libraries (e.g. `C:\Local\OpenSSL-Win64`).
- GMP version 4.1 (Please note that [MPIR](http://www.mpir.org/downloads.html) may be considered as a good Windows-ready alternative to  [GMP](https://gmplib.org/), as described [here](https://stackoverflow.com/questions/47359417/how-to-compile-gmp-for-windows-using-visual-studio.). In a future release of `teos` we will probably switch to MPIR, as it seems to be better suited for Windows).
  - Window binaries are available [here](https://cs.nyu.edu/~exact/core/gmp/index.html) (choose static ones for MinGW, i.e. `gmp-static-mingw-4.1.tar.gz`). You might need [7-Zip](http://www.7-zip.org/) to extract them.
  - Define an environment variable `GMP_DIR` pointing to the location you've chosen to store the GMP libraries (e.g. `C:\Local\gmp-static-mingw-4.1`).
- Secp256k1 - as there are no pre-compiled binaries available what you'll need to do is cross-compilation between Linux and Windows. This will be described in the next section. But first do these simple steps:
  - Create a location of your choice where Secp256k1 libraries will be stored. In our case it's `c:\Local\secp256k1`.
  - Define an environment variable `SECP256K1_DIR` pointing to the above directory.


#### Secp256k1 cross-compilation

[Secp256k1](https://github.com/bitcoin-core/secp256k1) is not available directly on Windows, so the only way to go is apply cross-compilation between Linux and Windows. For this purpose, you'll need access to a Linux environment. In our view, the easiest option is using [Windows Subsystem for Linux](https://msdn.microsoft.com/en-us/commandline/wsl/about), and the rest of this section is based on this choice.

If you haven't already done so, enable *Windows Subsystem for Linux* on your Windows machine, as described in [this guide](https://docs.microsoft.com/en-us/windows/wsl/install-win10).

While on Windows, run Ubuntu bash and start with making sure you are running Ubuntu 16.04:

```
lsb_release -a
```

And before you continue, update & upgrade Ubuntu:

```
sudo apt update
sudo apt full-upgrade
```

Make sure that you have `MinGW-64bit` installed:

```
sudo apt-get install mingw-w64
```

Make sure that you have `autoconf` installed: 

```
sudo apt-get install autoconf
```

Make sure that you have `make` installed: 

```
sudo apt-get install make
```

Make sure that you have `libtool` installed: 

```
sudo apt-get install libtool
```

Get a copy of the `secp256k1` repository and navigate to its folder:

```
git clone https://github.com/cryptonomex/secp256k1-zkp.git
cd secp256k1-zkp
```

Define those two variables:

```
export CFLAGS="-v"
export installDir=/mnt/c/Local/secp256k1/
```

Please note that the value of the variable `installDir` needs to match the Windows destination for your *Secp256k1* libraries, as described in the previous section. So probably you'll need to apply a different path than the one used above, unless you've chosen the same location as we did.

If you are doing this step not for the first time, you need to reset the workspace (otherwise skip this step):

```
make clean
```

And now you're ready to build and install *Secp256k1*:

```
./autogen.sh
./configure --host=x86_64-w64-mingw32 --prefix=${installDir}
make
make install
```

For convenience let's rename the `libsecp256k1` file to match Windows conventions:

```
mv ${installDir}/lib/libsecp256k1.a ${installDir}/lib/secp256k1.lib
```

Copy the library `libgcc` which comes as part of *Mingw*:

```
cp /usr/lib/gcc/x86_64-linux-gnu/7/libgcc.a ${installDir}/lib/gcc.lib
```

And finally, copy the executable `tests` which will be used for testing:

```
cp tests.exe ${installDir}/tests.exe
```

Before you exit Ubuntu bash, you might want to clean the workspace:

```
cd ..
rm -rf secp256k1-zkp
```

Open *PowerShell*, navigate to `SECP256K1_DIR` (in our case it's `C:\Local\secp256k1`) and run `tests.exe` to make sure there are no errors:

```
cd C:\Local\secp256k1
./tests.exe
```

#### Cloning the source code

Open *Visual Studio 2017 Developer Command Prompt*, navigate to a location of your choice and clone *teos* repository:

```
git clone https://github.com/tokenika/teos.git
```

#### Compilation

Still using *Visual Studio 2017 Developer Command Prompt*, navigate to the `teos\teos` folder and run the following commands:
```
cd teos\teos
mkdir bulid
cd build
cmake -G "Visual Studio 15 2017 Win64" ..
msbuild teos.sln
msbuild INSTALL.vcxproj
```

#### Testing on remote sever

Open *Power Shell* and navigate to the location of your *teos* repository, and then inside the repository navigate to the `teos\install\bin` folder:

```
cd teos\install\bin
```

And now you should be able to run `teos` and access EOS full node running on one of our servers:

```
./teos 198.100.148.136:8888 get info
```

Alternatively, you can use the predefined placeholder `tokenika` instead of  `198.100.148.136:8888`:

```
./teos tokenika get info
```
You can try simple queries, e.g.

```
./teos tokenika get block 25
```

```
##         block number: 25
##            timestamp: 2017-12-05T19:55:56
##     ref block prefix: 1139663381
```
Or you can test private key generation:

```
./teos tokenika create key
```

```
##             key name: default
##          private key: 5JyL28JPQbPTYwTpjKRcXvfj6nwUKgCmHnJaD28nmmWMpHXukVn
##           public key: EOS7TxBhoCwAWXoV8uhtgjz4inTLwiwcySvrVhGNYcjhw75wFJ9uA
```

## Conclusion

We dare to hope that this little work of ours could become an interesting alternative to the original `eosc` CLI, and maybe one day be included as part of EOS codebase.