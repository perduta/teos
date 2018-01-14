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


### `boost::asio::io_service` 

We've removed false redefinitions:

* `fc\include\fc\log\appender.hpp` [line 5]: 
```
#include <boost/asio.hpp>

// namespace boost { namespace asio { class io_service; } }
```
* `fc\include\fc\log\gelf_appender.hpp` [line6]:
```
#include <boost/asio.hpp>

// namespace boost { namespace asio { class io_service; } }
```
### `bool require_canonical` 

In `eos/libraries/fc/src/crypto/elliptic_openssl.cpp` we've added a missing definition:
```
compact_signature private_key::sign_compact( const fc::sha256& digest, bool require_canonical ) const
```
### `static_assert`
We've commented out `eos\libraries\fc\include\fc\io\raw.hpp` [lines 690 & 695]

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

`OpenSSL`, `secp256k1` and `GMP` compilations that we use are incompatible with the Visual Studio 2017, hence, they needed to be patched with additional libraries.

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