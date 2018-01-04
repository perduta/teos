add_library( eos_types STATIC
  asset.cpp
  public_key.cpp
  type_parser.cpp
  abi_serializer.cpp
  native.cpp
  types.eos
)

target_include_directories( eos_types
  PRIVATE
  ${Boost_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  ../fc/include
  ../utilities/include

  PUBLIC 
  include
)

target_link_libraries( eos_types 
  PRIVATE
  ${Boost_LIBRARIES} 
  ${OPENSSL_LIBRARIES}
  ${Secp256k1_LIBRARIES}
  ${GMP_LIBRARIES} 
  eos_utilities 
  fc 
)
  