set( LIB_NAME eos_types)

add_library( ${LIB_NAME} STATIC
  asset.cpp
  public_key.cpp
  type_parser.cpp
  abi_serializer.cpp
  native.cpp
  types.eos
)

target_include_directories( ${LIB_NAME}
  PUBLIC
  ${Boost_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  ../fc/include
  ../utilities/include

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
  eos_utilities 
  fc 
)

install( DIRECTORY include DESTINATION . )
install(
  TARGETS ${LIB_NAME} 
  LIBRARY DESTINATION lib 
  ARCHIVE DESTINATION lib
)