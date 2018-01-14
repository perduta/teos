# Amendments to EOS support libraries for Windows compatibility

In order to be able to manage things like private key generation, `teos` is dependent on three C++ libraries (namely `fc`, `types` & `utilities`), which are part of EOS codebase.

However, the current EOS codebase is not Windows-ready, i.e. it's impossible to compile it in Windows.

To make the above libraries (i.e.`fc`, `types` & `utilities`) compatible with Windows we needed to create clones of those libraries and introduce slight changes. The rest of this document describes the details of those amendments.

In the future we plan to turn these modifications into proper GitHub pull requests against EOS repository, so that those libraries can become platform-independent. But for the time being those amended clones are part of `teos` codebase.

---

###  `__int128`

The type `unsigned __int128` does not exist in Windows. Instead,  in `fc/uint128.hpp` we rely on a solution supported by the Boost library. Note that the same dependency is originally used in `fc/src/uint128.cpp`.

```
#include <boost/multiprecision/cpp_int.hpp>
........
........
typedef boost::multiprecision::uint128_t;
```
We've replaced all occurrences of `unsigned __int128` with `boost_uint128`.

---


### `boost::asio::io_service` 

We've removed unnecessary redefinitions:

* In `fc\include\fc\log\appender.hpp`, line 5: 
```
#include <boost/asio.hpp>

// namespace boost { namespace asio { class io_service; } }
```
* In `fc\include\fc\log\gelf_appender.hpp`, line 6:
```
#include <boost/asio.hpp>

// namespace boost { namespace asio { class io_service; } }
```
---

### `bool require_canonical` 

In `eos/libraries/fc/src/crypto/elliptic_openssl.cpp` we've fixed the `bool require_canonical` definition:
```
compact_signature private_key::sign_compact( const fc::sha256& digest, bool require_canonical ) const
```
---

### `static_assert`

In `eos\libraries\fc\include\fc\io\raw.hpp` we've commented out line 693:

```
template<typename Stream, typename T> void pack( Stream& s, const boost::multiprecision::number<T>& n ) {
//static_assert( sizeof( n ) == (std::numeric_limits<boost::multiprecision::number<T>>::digits+1)/8, "unexpected padding" );
      s.write( (const char*)&n, sizeof(n) );
 }
```

And line 697:

```
template<typename Stream, typename T> void unpack( Stream& s,  boost::multiprecision::number<T>& n ) {
//static_assert( sizeof( n ) == (std::numeric_limits<boost::multiprecision::number<T>>::digits+1)/8, "unexpected padding" );
      s.read( (char*)&n, sizeof(n) );
 }
```