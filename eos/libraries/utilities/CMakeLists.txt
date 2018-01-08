set( LIB_NAME eos_utilities)

add_library(${LIB_NAME} STATIC 
  key_conversion.cpp
  string_escape.cpp
  tempdir.cpp
  words.cpp
)

target_include_directories( ${LIB_NAME} 
  PUBLIC
  ${Boost_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  ../fc/include
  
  PUBLIC  
  include
)

target_link_libraries( ${LIB_NAME}
  PUBLIC
  ${Boost_LIBRARIES} 
  ${OPENSSL_LIBRARIES}
  ${Secp256k1_LIBRARIES}
  ${GMP_LIBRARIES}
  
  PUBLIC
  eos_types 
  fc 
)

install( DIRECTORY include DESTINATION . )
install(
  TARGETS ${LIB_NAME} 
  LIBRARY DESTINATION lib 
  ARCHIVE DESTINATION lib
)