add_library(eos_utilities STATIC 
  key_conversion.cpp
  string_escape.cpp
  tempdir.cpp
  words.cpp
)

target_include_directories( eos_utilities 
  PRIVATE
  ${Boost_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  ../fc/include
  
  PUBLIC  
  include
)

target_link_libraries( eos_utilities
  PRIVATE
  ${Boost_LIBRARIES} 
  ${OPENSSL_LIBRARIES}
  ${Secp256k1_LIBRARIES}
  ${GMP_LIBRARIES}   
  eos_types 
  fc 
)