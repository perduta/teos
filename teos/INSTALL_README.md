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

`Secp256k1` is not available immediately.

Now, we cannot offer anything else than a cross-compilation between Linux and Windows. To accomplish this process, you need a Linux environment, the [Windows Subsystem for Linux](https://msdn.microsoft.com/en-us/commandline/wsl/about), for example.
 

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
export installDir=/mnt/hgfs/C_INCLUDE/secp256k1/ 

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
Finally, define `SECP256K1_DIR` the Windows interpretation of `${installDir}` (for example, E:/C_INCLUDE/secp256k1)

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