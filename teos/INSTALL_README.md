# TEOS: Tokenika's alternative for EOS command line interface

## Dependencies

We assume that your computer is 64bit.

In its Linux version, TEOS needs `Boost`, `OpenSSL`, `GMP` and `Secp256k1` of the dependencies specified in the EOS documentation. 

However, as TEOS is born to live in Windows, it needs Windows-compiled dependencies. Some of them are easy:

* [Boost 1.64](#https://sourceforge.net/projects/boost/files/boost-binaries/), define `BOOST_LIBRARYDIR` (for example, %BOOST_ROOT%/lib64-msvc-14.1) and `BOOST_INCLUDEDIR` (where is boost directory, usualy %BOOST_ROOT%) (We do not want any higher `Boost' version since they are incompatible with the `CMake 3.8`);
* [OpenSSL](#https://indy.fulgan.com/SSL/openssl-1.0.2l-x64_86-win64.zip), define `OPENSSL_ROOT_DIR` (for example, %C_INCLUDE%/OpenSSL/Win64OpenSSL-1_0_2L);
* [GMP](#http://fsmath.mathematik.uni-dortmund.de/~mabshoff/?path=gmp-bin/4.2.2/) define `GMP_DIR` (for example, E:/C_INCLUDE/gmp)

[`MPIR`](#http://www.mpir.org/downloads.html) may be connsidered as a Windows-ready alternative to `GMP`. See also https://stackoverflow.com/questions/47359417/how-to-compile-gmp-for-windows-using-visual-studio.

## secp256k1 library for Windows

`Secp256k1` is not available immediately. Now, we cannot offer anything else than a cross-compilation between Linux and Windows. To accomplish this process, you need a Linux environment, the [Windows Subsystem for Linux](https://msdn.microsoft.com/en-us/commandline/wsl/about), for example.

```
export CFLAGS="-v" # let compiler be verbose
export installDir=/mnt/hgfs/C_INCLUDE/secp256k1/
export TEMP_DIR = (...) # chose a working directory  
```

Get a copy of the `secp256k1` repository:
```
cd ${TEMP_DIR}
git clone https://github.com/cryptonomex/secp256k1-zkp.git
cd secp256k1-zkp
```
Be sure that you have `MinGW-64bit` installed:
```
sudo apt-get install mingw-w64
```
Build and install `secp256k1` for Windows:
```
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
```
rm -rf cd ${TEMP_DIR}/secp256k1-zkp # clean the workshop
```
Finally, define `SECP256K1_DIR` the Windows interpretation of `${installDir}` (for example, E:/C_INCLUDE/secp256k1)

## secp256k1 library for Windows - erratum

In fact, there is available a better Windows source of the `secp256k1` library.

Define `SECP256K1_DIR` (for example, E:/C_INCLUDE/secp256k1).
Use Windows `powershell`:
```
git clone https://github.com/evoskuil/secp256k1/
cd ./secp256k1/builds/msvc/vs217/
msbuild secp256k1.sln /property:Configuration=StaticRelease /property:Platform=x64
mkdir $Env:SECP256K1_DIR/include
$Env:SECP256K1_DIR/lib/secp256k1.lib
cp ../../../include/* $Env:SECP256K1_DIR/include/
mkdir $Env:SECP256K1_DIR/lib
cp ../../../bin/x64/Release/v141/static/secp256k1.lib 
```
However, the `EOS` libraries are not ready for this improvement now, we will tune them later on. It follows a list of compile errors:

```
"E:\Workspaces\EOS\Pentagon\teos\buildWindows\teos.sln" (default target) (1) ->
"E:\Workspaces\EOS\Pentagon\teos\buildWindows\ALL_BUILD.vcxproj.metaproj" (default target) (2) ->
"E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\types\eos_types.vcxproj.metaproj" (default targ
et) (4) ->
"E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\utilities\eos_utilities.vcxproj.metaproj" (defa
ult target) (5) ->
"E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\fc\fc.vcxproj.metaproj" (default target) (6) ->
"E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\fc\fc.vcxproj" (default target) (7) ->
(ClCompile target) ->
  e:\workspaces\eos\pentagon\teos\libraries\fc\src\crypto\_elliptic_impl_priv.hpp(11): error C4430: mis
sing type specifier - int assumed. Note: C++ does not support default-int [E:\Workspaces\EOS\Pentagon\t
eos\buildWindows\libraries\fc\fc.vcxproj]
  e:\workspaces\eos\pentagon\teos\libraries\fc\src\crypto\_elliptic_impl_priv.hpp(11): error C2143: syn
tax error: missing ';' before '*' [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\fc\fc.vcxproj
]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_impl_priv.cpp(75): error C2660: 'sec
p256k1_ec_pubkey_create': function does not take 5 arguments [E:\Workspaces\EOS\Pentagon\teos\buildWind
ows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_impl_priv.cpp(85): error C2198: 'con
st secp256k1_nonce_function': too few arguments for call [E:\Workspaces\EOS\Pentagon\teos\buildWindows\
libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_impl_priv.cpp(96): error C3861: 'sec
p256k1_ecdsa_sign_compact': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\librarie
s\fc\fc.vcxproj]
  e:\workspaces\eos\pentagon\teos\libraries\fc\src\crypto\_elliptic_impl_priv.hpp(11): error C4430: mis
sing type specifier - int assumed. Note: C++ does not support default-int [E:\Workspaces\EOS\Pentagon\t
eos\buildWindows\libraries\fc\fc.vcxproj]
  e:\workspaces\eos\pentagon\teos\libraries\fc\src\crypto\_elliptic_impl_priv.hpp(11): error C2143: syn
tax error: missing ';' before '*' [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\fc\fc.vcxproj
]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(26): error C4430: missnst secp256k1_context *,unsigned char *,const unsigned char *)': can
not convert argument 1 from 'int *' to 'const secp256k1_context *' [E:\Workspaces\EOS\Pentagon\teos\bui
ldWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(442): error C3861: 'se
cp256k1_ecdsa_recover_compact': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libr
aries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(461): error C3861: 'se
cp256k1_pedersen_commit': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\
fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(470): error C3861: 'se
cp256k1_pedersen_blind_sum': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\librari
es\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(482): error C3861: 'se
cp256k1_pedersen_verify_tally': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libr
aries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(487): error C3861: 'se
cp256k1_rangeproof_verify': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\librarie
s\fc\fc.vcxproj]nst secp256k1_context *,unsigned char *,const unsigned char *)': can
not convert argument 1 from 'int *' to 'const secp256k1_context *' [E:\Workspaces\EOS\Pentagon\teos\bui
ldWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(442): error C3861: 'se
cp256k1_ecdsa_recover_compact': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libr
aries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(461): error C3861: 'se
cp256k1_pedersen_commit': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\
fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(470): error C3861: 'se
cp256k1_pedersen_blind_sum': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\librari
es\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(482): error C3861: 'se
cp256k1_pedersen_verify_tally': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libr
aries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(487): error C3861: 'se
cp256k1_rangeproof_verify': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\librarie
s\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(502): error C3861: 'se
cp256k1_rangeproof_sign': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\
fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(525): error C3861: 'se
cp256k1_rangeproof_rewind': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\librarie
s\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(544): error C3861: 'se
cp256k1_rangeproof_info': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\
fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(502): error C3861: 'se
cp256k1_rangeproof_sign': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\
fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(525): error C3861: 'senst secp256k1_context *,unsigned char *,const unsigned char *)': can
not convert argument 1 from 'int *' to 'const secp256k1_context *' [E:\Workspaces\EOS\Pentagon\teos\bui
ldWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(442): error C3861: 'se
cp256k1_ecdsa_recover_compact': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libr
aries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(461): error C3861: 'se
cp256k1_pedersen_commit': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\
fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(470): error C3861: 'se
cp256k1_pedersen_blind_sum': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\librari
es\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(482): error C3861: 'se
cp256k1_pedersen_verify_tally': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libr
aries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(487): error C3861: 'se
cp256k1_rangeproof_verify': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\librarie
s\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(502): error C3861: 'se
cp256k1_rangeproof_sign': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\
fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(525): error C3861: 'se
cp256k1_rangeproof_rewind': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\librarie
s\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(544): error C3861: 'se
cp256k1_rangeproof_info': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\
fc\fc.vcxproj]
cp256k1_rangeproof_rewind': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\librarie
s\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(544): error C3861: 'se
cp256k1_rangeproof_info': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\
fc\fc.vcxproj]
ing type specifier - int assumed. Note: C++ does not support default-int [E:\Workspaces\EOS\Pentagon\te
os\buildWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(26): error C2143: synt
ax error: missing ';' before '*' [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(26): error C2086: 'con
st int fc::ecc::detail::secp256k1_context_t': redefinition [E:\Workspaces\EOS\Pentagon\teos\buildWindow
s\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(26): error C2059: synt
ax error: '{' [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(26): error C2143: synt
ax error: missing ';' before '{' [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(26): error C2447: '{':
 missing function header (old-style formal list?) [E:\Workspaces\EOS\Pentagon\teos\buildWindows\librari
es\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(32): error C4430: missnst secp256k1_context *,unsigned char *,const unsigned char *)': can
not convert argument 1 from 'int *' to 'const secp256k1_context *' [E:\Workspaces\EOS\Pentagon\teos\bui
ldWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(442): error C3861: 'se
cp256k1_ecdsa_recover_compact': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libr
aries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(461): error C3861: 'se
cp256k1_pedersen_commit': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\
fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(470): error C3861: 'se
cp256k1_pedersen_blind_sum': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\librari
es\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(482): error C3861: 'se
cp256k1_pedersen_verify_tally': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libr
aries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(487): error C3861: 'se
cp256k1_rangeproof_verify': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\librarie
s\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(502): error C3861: 'se
cp256k1_rangeproof_sign': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\
fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(525): error C3861: 'se
cp256k1_rangeproof_rewind': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\librarie
s\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(544): error C3861: 'se
cp256k1_rangeproof_info': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\
fc\fc.vcxproj]
ing type specifier - int assumed. Note: C++ does not support default-int [E:\Workspaces\EOS\Pentagon\te
os\buildWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(32): error C2143: synt
ax error: missing ';' before '*' [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(32): error C2065: 'ctx
': undeclared identifier [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(34): error C2065: 'ctx
': undeclared identifier [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(72): error C2660: 'sec
p256k1_ec_pubkey_tweak_mul': function does not take 4 arguments [E:\Workspaces\EOS\Pentagon\teos\buildW
indows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(107): error C2660: 'se
cp256k1_ec_pubkey_tweak_add': function does not take 4 arguments [E:\Workspaces\EOS\Pentagon\teos\build
Windows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(129): error C3861: 'se
cp256k1_ec_pubkey_decompress': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libra
ries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(167): error C3861: 'se
cp256k1_ecdsa_recover_compact': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libr
aries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(183): error C2660: 'se
cp256k1_ec_pubkey_tweak_add': function does not take 4 arguments [E:\Workspaces\EOS\Pentagon\teos\build
Windows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(299): error C2664: 'in
t secp256k1_ec_privkey_tweak_mul(const secp256k1_context *,unsigned char *,const unsigned char *)': can
not convert argument 1 from 'int *' to 'const secp256k1_context *' [E:\Workspaces\EOS\Pentagon\teos\bui
ldWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(302): error C2660: 'se
cp256k1_ec_pubkey_tweak_mul': function does not take 4 arguments [E:\Workspaces\EOS\Pentagon\teos\build
Windows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(313): error C2664: 'in
t secp256k1_ec_privkey_tweak_mul(const secp256k1_context *,unsigned char *,const unsigned char *)': can
not convert argument 1 from 'int *' to 'const secp256k1_context *' [E:\Workspaces\EOS\Pentagon\teos\bui
ldWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(317): error C2660: 'se
cp256k1_ec_pubkey_tweak_mul': function does not take 4 arguments [E:\Workspaces\EOS\Pentagon\teos\build
Windows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(329): error C2660: 'se
cp256k1_ec_pubkey_tweak_add': function does not take 4 arguments [E:\Workspaces\EOS\Pentagon\teos\build
Windows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(335): error C2664: 'in
t secp256k1_ec_privkey_tweak_mul(const secp256k1_context *,unsigned char *,const unsigned char *)': can
not convert argument 1 from 'int *' to 'const secp256k1_context *' [E:\Workspaces\EOS\Pentagon\teos\bui
ldWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(340): error C2660: 'se
cp256k1_ec_pubkey_tweak_mul': function does not take 4 arguments [E:\Workspaces\EOS\Pentagon\teos\build
Windows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(356): error C2664: 'in
t secp256k1_ec_privkey_tweak_add(const secp256k1_context *,unsigned char *,const unsigned char *)': can
not convert argument 1 from 'int *' to 'const secp256k1_context *' [E:\Workspaces\EOS\Pentagon\teos\bui
ldWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(383): error C2664: 'in
t secp256k1_ec_privkey_tweak_mul(const secp256k1_context *,unsigned char *,const unsigned char *)': can
not convert argument 1 from 'int *' to 'const secp256k1_context *' [E:\Workspaces\EOS\Pentagon\teos\bui
ldWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(384): error C2664: 'in
t secp256k1_ec_privkey_tweak_add(const secp256k1_context *,unsigned char *,const unsigned char *)': can
not convert argument 1 from 'int *' to 'const secp256k1_context *' [E:\Workspaces\EOS\Pentagon\teos\bui
ldWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(401): error C2664: 'in
t secp256k1_ec_privkey_tweak_mul(const secp256k1_context *,unsigned char *,const unsigned char *)': can
not convert argument 1 from 'int *' to 'const secp256k1_context *' [E:\Workspaces\EOS\Pentagon\teos\bui
ldWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(410): error C2664: 'in
t secp256k1_ec_privkey_tweak_mul(const secp256k1_context *,unsigned char *,const unsigned char *)': can
not convert argument 1 from 'int *' to 'const secp256k1_context *' [E:\Workspaces\EOS\Pentagon\teos\bui
ldWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(411): error C2664: 'in
t secp256k1_ec_privkey_tweak_add(const secp256k1_context *,unsigned char *,const unsigned char *)': can
not convert argument 1 from 'int *' to 'const secp256k1_context *' [E:\Workspaces\EOS\Pentagon\teos\bui
ldWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(430): error C2664: 'in
t secp256k1_ec_privkey_tweak_mul(const secp256k1_context *,unsigned char *,const unsigned char *)': can
not convert argument 1 from 'int *' to 'const secp256k1_context *' [E:\Workspaces\EOS\Pentagon\teos\bui
ldWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(431): error C2664: 'in
t secp256k1_ec_privkey_tweak_add(const secp256k1_context *,unsigned char *,const unsigned char *)': can
not convert argument 1 from 'int *' to 'const secp256k1_context *' [E:\Workspaces\EOS\Pentagon\teos\bui
ldWindows\libraries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(442): error C3861: 'se
cp256k1_ecdsa_recover_compact': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libr
aries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(461): error C3861: 'se
cp256k1_pedersen_commit': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\
fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(470): error C3861: 'se
cp256k1_pedersen_blind_sum': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\librari
es\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(482): error C3861: 'se
cp256k1_pedersen_verify_tally': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libr
aries\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(487): error C3861: 'se
cp256k1_rangeproof_verify': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\librarie
s\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(502): error C3861: 'se
cp256k1_rangeproof_sign': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\
fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(525): error C3861: 'se
cp256k1_rangeproof_rewind': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\librarie
s\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(544): error C3861: 'se
cp256k1_rangeproof_info': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\
fc\fc.vcxproj]
```

## Windows compilation of a selection of the EOS libraries

The current EOS code is not Windows-ready. We have extracted fragments that it need and corrected few problems. It follows the list of changes.

###  __int128

The type `unsigned __int128` does not exist in Windows. Instead,  we use a `boost` solution in `fc/uint128.hpp`. Note that the same dependency is originally used in `fc/src/uint128.cpp`.

```
#include <boost/multiprecision/cpp_int.hpp>
........
........
typedef boost::multiprecision::uint128_t  ;
```
We replace all the occurrences `unsigned __int128` with `boost_uint128`.


### boost::asio::io_service redefinitions

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
### Missing `bool require_canonical` 

In eos/libraries/fc/src/crypto/elliptic_openssl.cpp
```
compact_signature private_key::sign_compact( const fc::sha256& digest, bool require_canonical)const
```
### static_assert
Comment out eos\libraries\fc\include\fc\io\raw.hpp [690] and [695]

Include eos\libraries\fc\include\fc\io\raw.hpp [692]:
```
    template<typename Stream, typename T> void pack( Stream& s, const boost::multiprecision::number<T>& n ) {
#ifdef _MSC_VER
      /*
      Originally, static assert:
      static_assert( sizeof( n ) == (std::numeric_limits<boost::multiprecision::number<T>>::digits+1)/8, "unexpected padding" );
      
      With Windows, assertion does not hold. Replasing it with runtine checking:
      */
      if (sizeof(n) != (std::numeric_limits<boost::multiprecision::number<T>>::digits + 1) / 8) {
        throw std::runtime_error("unexpected padding");
      }
#else
      static_assert( sizeof( n ) == (std::numeric_limits<boost::multiprecision::number<T>>::digits+1)/8, "unexpected padding" );
#endif // _MSC_VER
      s.write( (const char*)&n, sizeof(n) );
    }
    template<typename Stream, typename T> void unpack( Stream& s,  boost::multiprecision::number<T>& n ) {
          
#ifdef _MSC_VER
      /*
      Originally, static assert:
      static_assert( sizeof( n ) == (std::numeric_limits<boost::multiprecision::number<T>>::digits+1)/8, "unexpected padding" );
      
      With Windows, assertion does not hold. Replasing it with runtine checking:
      */
      if (sizeof(n) != (std::numeric_limits<boost::multiprecision::number<T>>::digits + 1) / 8) {
        throw std::runtime_error("unexpected padding");
      }
#else
      static_assert( sizeof( n ) == (std::numeric_limits<boost::multiprecision::number<T>>::digits+1)/8, "unexpected padding" );
#endif // _MSC_VER
      s.read( (char*)&n, sizeof(n) );
    }
```

## Additional Windows libraries

All the listed dependencies are automatically managed with the `CMakeLists` files. Here, they are named for the record sake.

`OpenSSL` needs `$(WindowsSDK_LibraryPath)\x64\crypt32.lib`.

`OpenSSL`, `secp256k1` and `GMP` compilations that we use are incompatible with the Visual Studio 2017, hence, they have to be patched with additional libraries.

```
$(C_INCLUDE)\secp256k1\lib\gcc.lib # that is /usr/lib/gcc/x86_64-linux-gnu/7/libgcc.a, it fixes unresolved ___chkstk_ms error
$(C_INCLUDE)\\lib\msvcrt.lib # unresolved  __iob_func error
$(MS_BUILD)\..\..\..\..\VC\Tools\MSVC\14.12.25827\lib\x64\legacy_stdio_definitions.lib # unresolved imp_fprintf error
$(WindowsSDK_LibraryPath)\x64\crypt32.lib # unresolved __imp_CertOpenStore error
Userenv.lib # unresolved GetUserProfileDirectoryW error
```
###  unresolved external symbol __imp___iob_func

The problem is explained [there](#https://stackoverflow.com/questions/30412951/unresolved-external-symbol-imp-fprintf-and-imp-iob-func-sdl2).

An not ideal solution, one adequate to our purpose yet, is defining a dummy `__imp___iob_func` (any functional definition is impossible, rather):

```
static FILE arr[3];
extern "C" FILE*  __cdecl __iob_func(void) {
  throw std::runtime_error("See https://stackoverflow.com/questions/30412951/unresolved-external-symbol-imp-fprintf-and-imp-iob-func-sdl2 and https://msdn.microsoft.com/en-us/library/bb531344.aspx#BK_CRT"  );
  return arr;
}
```
We place the patch, (temporarily, perhaps) in the `main` of the `teos` program.

## Visual Studio precompiler macros

All the listed macros are automatically managed with the `CMakeLists` files. Here, they are named for the record sake.

* _CRT_SECURE_NO_WARNINGS
* -D_WIN32_WINNT=0x0501