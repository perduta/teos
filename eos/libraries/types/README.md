# Compiling for Windows eos/libraries/types

## __int128

The type `unsigned __int128` does not exist in Windows. Instead,  we use in `native.hpp` a boost solution:

eos\types\native.hpp [66]:
```
//using uint128_t = unsigned __int128; /// native clang/gcc 128 intrinisic
using uint128_t = boost::multiprecision::uint128_t; 
```

## static_assert

E:\Workspaces\EOS\Pentagon\eos\libraries\fc\include\fc\io\raw.hpp [692]
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