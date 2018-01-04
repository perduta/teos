
# Compiling for Windows eos/libraries/fc

## utf8_validator.hpp

`eos\libraries\fc\src\utf8.cpp` depend on `eos\libraries\fc\vendor\websocketpp\websocketpp\utf8_validator.hpp' 
and this is the only connection from the `fc` main source directory and the `vendor` directory. We copy this header 
to `eos\libraries\fc\include\fc`.
## __int128

The type `unsigned __int128` does not exist in Windows. Instead,  we use in `fc/uint128.hpp` a boost solution:

```
#include <boost/multiprecision/cpp_int.hpp>
........
........
typedef boost::multiprecision::uint128_t  ;
```
Note that the same dependency is originally used in `fc/src/uint128.cpp`.

We replace all the occurrences `unsigned __int128` with `boost_uint128`.


## boost::asio::io_service redefinitions

We remove false redefinitions:

* fc\include\fc\log\appender.hpp [5]: 
```
#include <boost/asio.hpp>

// namespace boost { namespace asio { class io_service; } }
```
* fc\include\fc\log\gelf_appender.hpp [6]:
```
#include <boost/asio.hpp>

// namespace boost { namespace asio { class io_service; } }
```
## eos/libraries/fc/src/crypto/elliptic_openssl.cpp

Missing `bool require_canonical` (which argument is not used):
```
compact_signature private_key::sign_compact( const fc::sha256& digest, bool require_canonical)const
```

## secp256k1 library for Ubuntu

```
cd \tmp
sudo git clone https://github.com/cryptonomex/secp256k1-zkp.git
cd secp256k1-zkp
sudo ./autogen.sh
sudo ./configure
sudo make
sudo make install
# /usr/local/lib/libsecp256k1.a
# /usr/local/lib/libsecp256k1.a
sudo rm -rf cd /tmp/secp256k1-zkp
```

## secp256k1 library for Windows

This section deserves its own publication as I could not find any clear guide on Google.

Get a copy of the `secp256k1` repository:
```
git clone https://github.com/cryptonomex/secp256k1-zkp.git
```
Be sure that you have `MinGW-64bit` installed:
```
sudo apt-get install mingw-w64
```
Build and install `secp256k1` for Windows:
```
export CFLAGS="-v" # let compiler be verbose
export installDir=/mnt/hgfs/C_INCLUDE/secp256k1/ # this directory is referenced in VS

./autogen.sh
make clean # if rebuilding
./configure --host=x86_64-w64-mingw32 --prefix=${installDir}
make
make install
./tests.exe

cp ${installDir}/lib/libsecp256k1.a secp256k1.lib

# let other dependencies of the tests.exe be at hand:
cp /usr/x86_64-w64-mingw32/lib/libmingw32.a ${installDir}/lib/mingw32.lib
cp /usr/lib/gcc/x86_64-linux-gnu/7/libgcc.a ${installDir}/lib/gcc.lib
cp /usr/lib/gcc/x86_64-w64-mingw32/6.3-win32/libgcc_eh.a ${installDir}/lib/gcc_eh.lib
cp /usr/x86_64-w64-mingw32/lib/libmoldname.a ${installDir}/lib/moldname.lib
cp /usr/x86_64-w64-mingw32/lib/libmingwex.a ${installDir}/lib/mingwex.lib
cp /usr/x86_64-w64-mingw32/lib/libmsvcrt.a ${installDir}/lib/msvcrt.lib
cp /usr/x86_64-w64-mingw32/lib/libadvapi32.a ${installDir}/lib/advapi32.lib
cp /usr/x86_64-w64-mingw32/lib/libshell32.a ${installDir}/lib/shell32.lib
cp /usr/x86_64-w64-mingw32/lib/libuser32.a ${installDir}/lib/user32.lib
cp /usr/x86_64-w64-mingw32/lib/libkernel32.a ${installDir}/lib/kernel32.lib
```
If used in an application, both `secp256k1` and `OpenSSL` will need additional dependencies:

```
$(OPENSSL_ROOT_DIR)/lib/VC/static/libeay32MT.lib
$(SolutionDir)lib/fc.lib
$(SolutionDir)lib/utilities.lib
$(C_INCLUDE)\secp256k1\lib\libsecp256k1.a
$(C_INCLUDE)\secp256k1\lib\libgcc.a
$(WindowsSDK_LibraryPath)\x64\crypt32.lib
$(MS_BUILD)\..\..\..\..\VC\Tools\MSVC\14.12.25827\lib\x64\legacy_stdio_definitions.lib
$(WindowsSDK_LibraryPath)\x64\crypt32.lib # unresolved __imp_CertOpenStore error
```
## OpenSSL

Get OpenSSL for Windows, Properties > Linker > Input > Additional Dependencies:
$(C_INCLUDE)/openssl-1.0.1q-vs2015/lib/libeay32MT.lib

## __iob_func

`secp256k1.lib(libsecp256k1_la-secp256k1.o) : error LNK2019: unresolved external symbol __imp___iob_func referenced in function checked_malloc.part.10`

The problem is explained [there](#https://stackoverflow.com/questions/30412951/unresolved-external-symbol-imp-fprintf-and-imp-iob-func-sdl2)
A solution is to define `__iob_func`:
```
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }
```
As I put declaration (temporarily, perhaps) in `fc/include/fc/crypto/openssl.hpp [12]`:
```
#ifdef _MSC_VER
// secp256k1 needs it, if compiled with mingw, at least.
extern "C" { FILE __iob_func[]; }
#endif // _MSC_VER
```
Accordingly, definition went to `fc/src/crypto/openssl.cpp [11]:`:
```
#ifdef _MSC_VER
// secp256k1 needs it, if compiled with mingw, at least.
FILE __iob_func[3] = { *stdin,*stdout,*stderr };
#endif // _MSC_VER
```
### No such file or directory building project "project_name.vcxproj" -- FAILED

Error message like this:
```
c1 : fatal error C1083: Cannot open source file: '..\..\..\..\..\..\C_INCLUDE\OpenSSL\Win64OpenSSL-1_0_2L\include\openssl\applink.c': No such file or directory
```
... is issued when a VS project, having included header files mixed with source ones, is moved. The only method to cure it is to edit the project definition file, excluding offending entries.


