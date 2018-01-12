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