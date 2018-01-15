### gmp and secp256k1

gmp and secp256k1 libraries are not used if 
```
set( EOS_ECC_IMPL openssl ) ## secp256k1 or openssl or mixed
```
However, this does not work:
```
libraries/fc/libfc.a(elliptic_common.cpp.o): In function `fc::ecc::extended_public_key::derive_child(int) const':
elliptic_common.cpp:(.text+0x2e43): undefined reference to `fc::ecc::extended_public_key::derive_normal_child(int) const'
libraries/fc/libfc.a(elliptic_common.cpp.o): In function `fc::ecc::extended_public_key::from_base58(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)':
elliptic_common.cpp:(.text+0x3446): undefined reference to `fc::ecc::extended_public_key::extended_public_key(fc::ecc::public_key const&, fc::sha256 const&, int, int, unsigned char)'
libraries/fc/libfc.a(elliptic_common.cpp.o): In function `fc::ecc::extended_private_key::get_extended_public_key() const':
elliptic_common.cpp:(.text+0x35bf): undefined reference to `fc::ecc::extended_public_key::extended_public_key(fc::ecc::public_key const&, fc::sha256 const&, int, int, unsigned char)'
libraries/fc/libfc.a(elliptic_common.cpp.o): In function `fc::ecc::extended_public_key::generate_p(int) const':
elliptic_common.cpp:(.text+0x3653): undefined reference to `fc::ecc::extended_public_key::derive_normal_child(int) const'
libraries/fc/libfc.a(elliptic_common.cpp.o): In function `fc::ecc::extended_public_key::generate_q(int) const':
elliptic_common.cpp:(.text+0x36f9): undefined reference to `fc::ecc::extended_public_key::derive_normal_child(int) const'
libraries/fc/libfc.a(elliptic_common.cpp.o): In function `fc::ecc::extended_private_key::derive_normal_child(int) const':
elliptic_common.cpp:(.text+0x38fa): undefined reference to `fc::ecc::extended_private_key::private_derive_rest(fc::sha512 const&, int) const'
libraries/fc/libfc.a(elliptic_common.cpp.o): In function `fc::ecc::extended_private_key::derive_hardened_child(int) const':
elliptic_common.cpp:(.text+0x3a7a): undefined reference to `fc::ecc::extended_private_key::private_derive_rest(fc::sha512 const&, int) const'
libraries/fc/libfc.a(elliptic_common.cpp.o): In function `fc::ecc::extended_private_key::from_base58(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)':
elliptic_common.cpp:(.text+0x42f8): undefined reference to `fc::ecc::extended_private_key::extended_private_key(fc::ecc::private_key const&, fc::sha256 const&, int, int, unsigned char)'
libraries/fc/libfc.a(elliptic_common.cpp.o): In function `fc::ecc::extended_private_key::generate_master(char const*, unsigned int)':
elliptic_common.cpp:(.text+0x4556): undefined reference to `fc::ecc::extended_private_key::extended_private_key(fc::ecc::private_key const&, fc::sha256 const&, int, int, unsigned char)'
```

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
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(461): error C3861: 
  'secp256k1_pedersen_commit': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\
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
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(525): error C3861: 
  'secp256k1_rangeproof_rewind': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\librarie
s\fc\fc.vcxproj]
  E:\Workspaces\EOS\Pentagon\teos\libraries\fc\src\crypto\elliptic_secp256k1.cpp(544): error C3861: 
  'secp256k1_rangeproof_info': identifier not found [E:\Workspaces\EOS\Pentagon\teos\buildWindows\libraries\
fc\fc.vcxproj]
```
## Additional Windows libraries

All the listed dependencies are automatically managed with the `CMakeLists` files. Here, they are named for the record sake.

`OpenSSL` needs `$(WindowsSDK_LibraryPath)\x64\crypt32.lib`.

`OpenSSL`, `secp256k1` and `GMP` compilations that we use are incompatible with the Visual Studio 2017, hence, they needed to be patched with additional libraries.

```
$(C_INCLUDE)\secp256k1\lib\gcc.lib # that is /usr/lib/gcc/x86_64-linux-gnu/7/libgcc.a, it fixes unresolved ___chkstk_ms error
$(C_INCLUDE)\\lib\msvcrt.lib # unresolved  __iob_func error
$(MS_BUILD)\..\..\..\..\VC\Tools\MSVC\14.12.25827\lib\x64\legacy_stdio_definitions.lib # unresolved imp_fprintf error
$(WindowsSDK_LibraryPath)\x64\crypt32.lib # unresolved __imp_CertOpenStore error
Userenv.lib # unresolved GetUserProfileDirectoryW error
```

### unresolved external symbol __imp___iob_func

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

- _CRT_SECURE_NO_WARNINGS
- -D_WIN32_WINNT=0x0501

## secp256k1 library for Windows (1)
[See](#cp ${installDir}/lib/libsecp256k1.a ${installDir}/lib/secp256k1.lib) [or](#cp ${installDir}/lib/libsecp256k1.a ${installDir}/lib/secp256k1.lib)
Install MSYS2.

Use `mingw64.exe` shell:
```
pacman -S make
pacman -S autoconf
pacman -S perl
pacman -S automake
pacman -S libtool
pacman -S mingw-w64-x86_64-toolchain
	Enter a selection (default=all): 3
```
```
export CFLAGS="-v" # let compiler be verbose
export installDir=/e/C_INCLUDE/secp256k1/
export TEMP_DIR = (...) # chose a working directory 
```
Get a copy of the `secp256k1` repository:
```
cd ${TEMP_DIR}
git clone https://github.com/cryptonomex/secp256k1-zkp.git
cd secp256k1-zkp
```
```
cd ${TEMP_DIR}/secp256k1-zkp/
./autogen.sh
./configure --prefix=${installDir}
make
make install
cp ${installDir}/lib/libsecp256k1.a ${installDir}/lib/secp256k1.lib
cp /mingw64/lib/gcc/x86_64-w64-mingw32/7.2.0/libgcc.a ${installDir}/lib/gcc.lib
cp /mingw64/x86_64-w64-mingw32/lib/libmingwex.a ${installDir}/lib/mingwex.lib
cp /mingw64/x86_64-w64-mingw32/lib/libmsvcrt.a ${installDir}/lib/msvcrt.lib #klops printf already defined in teoslib.lib(teos_command.obj)
cp /mingw64/lib/libgmp.a ${installDir}/../gmp/lib/gmp.lib 
```