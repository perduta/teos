if( NOT EOS_ECC_IMPL )
  set( EOS_ECC_IMPL openssl)
endif( NOT EOS_ECC_IMPL )

if( EOS_ECC_IMPL STREQUAL secp256k1 )
  message( STATUS "EOS_ECC_IMPL is secp256k1" )
endif()

if( EOS_ECC_IMPL STREQUAL openssl )
  message( STATUS "EOS_ECC_IMPL is openssl" )
  set( ECC_REST src/crypto/elliptic_impl_pub.cpp )
else( EOS_ECC_IMPL STREQUAL openssl )
  if( EOS_ECC_IMPL STREQUAL mixed )
    message( STATUS "EOS_ECC_IMPL is mixed" )
    set( ECC_REST src/crypto/elliptic_impl_priv.cpp src/crypto/elliptic_impl_pub.cpp )
  else( EOS_ECC_IMPL STREQUAL mixed )
    set( ECC_REST src/crypto/elliptic_impl_priv.cpp )
  endif( EOS_ECC_IMPL STREQUAL mixed )
endif( EOS_ECC_IMPL STREQUAL openssl )

add_library( fc STATIC
  src/uint128.cpp
  src/real128.cpp
  src/variant.cpp
  src/exception.cpp
  src/variant_object.cpp
  src/string.cpp
  src/shared_ptr.cpp
  src/time.cpp
  src/filesystem.cpp 
  src/utf8.cpp

  src/io/datastream.cpp
  src/io/json.cpp
  src/io/varint.cpp
  src/io/fstream.cpp
  src/io/console.cpp

  src/log/log_message.cpp
  src/log/logger.cpp
  src/log/appender.cpp
  src/log/console_appender.cpp
  src/log/gelf_appender.cpp
  src/log/logger_config.cpp

  src/crypto/_digest_common.cpp
  src/crypto/aes.cpp
  src/crypto/crc.cpp
  src/crypto/city.cpp
  src/crypto/base36.cpp
  src/crypto/base58.cpp
  src/crypto/base64.cpp
  src/crypto/bigint.cpp
  src/crypto/hex.cpp
  src/crypto/sha1.cpp
  src/crypto/openssl.cpp
  src/crypto/ripemd160.cpp
  src/crypto/sha256.cpp
  src/crypto/sha224.cpp
  src/crypto/sha512.cpp
  src/crypto/dh.cpp
  src/crypto/blowfish.cpp
  src/crypto/elliptic_common.cpp
  ${ECC_REST}
  src/crypto/elliptic_${EOS_ECC_IMPL}.cpp
  src/crypto/rand.cpp

  src/interprocess/file_mapping.cpp
  src/interprocess/mmap_struct.cpp

  src/compress/smaz.cpp
  src/compress/zlib.cpp

  src/network/ip.cpp
  src/network/resolve.cpp
  src/network/udp_socket.cpp
  src/network/url.cpp
)

#if( EOS_BUILD_RANGE STREQUAL eosc OR EOS_BUILD_RANGE STREQUAL all )
#target_sources( fc PUBLIC
#)
#endif( EOS_BUILD_RANGE STREQUAL eosc OR EOS_BUILD_RANGE STREQUAL all )

#if( EOS_BUILD_RANGE STREQUAL all )
#  target_sources( fc 
#  PUBLIC
#)
#endif( EOS_BUILD_RANGE STREQUAL all )

target_include_directories(fc 
  PRIVATE 
  ${Boost_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  ${SECP256K1_INCLUDE_DIR}
  
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/websocketpp
)

target_link_libraries( fc 
  PRIVATE
  ${Boost_LIBRARIES} 
  ${OPENSSL_LIBRARIES}
  ${CMAKE_DL_LIBS}
)

if( NOT (APPLE OR WIN32) )
  set( pthread_library pthread )
  target_link_libraries( fc PUBLIC rt )
endif( NOT (APPLE OR WIN32) )

if( NOT EOS_ECC_IMPL STREQUAL openssl )
  target_link_libraries( fc 
    PRIVATE 
    ${SECP256K1_LIBRARIES}
    ${GMP_LIBRARIES}
  )
endif( NOT EOS_ECC_IMPL STREQUAL openssl )
