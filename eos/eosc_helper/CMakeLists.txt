set( LIB_NAME "eosc_helper")

add_library( ${LIB_NAME} STATIC
  eosc_helper.cpp
)

target_include_directories( ${LIB_NAME} 
  PRIVATE
  ${Boost_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  ../libraries/fc/include
  ../libraries/types/include
  ../libraries/utilities/include

  PUBLIC
  include
)

if( NOT EOS_ECC_IMPL STREQUAL openssl )
  target_include_directories( ${LIB_NAME}
    PRIVATE
    ${SECP256K1_INCLUDE_DIR}
    ${GMP_INCLUDE_DIR}
  )
endif()

target_link_libraries( ${LIB_NAME}
  PRIVATE
  ${Boost_LIBRARIES} 
  ${OPENSSL_LIBRARIES}
  fc
  eos_types
  eos_utilities
)

if( NOT EOS_ECC_IMPL STREQUAL openssl )
  target_link_libraries( ${LIB_NAME}
    PRIVATE
    ${SECP256K1_LIBRARIES}
    ${GMP_LIBRARIES}
  )
endif()

if( WIN32 )
  target_include_directories( ${LIB_NAME}
    PUBLIC
    ${OPENSSL_INCLUDE_DIR}
  )

  if( NOT EOS_ECC_IMPL STREQUAL openssl )
    target_link_libraries( ${LIB_NAME}
      PRIVATE
      ${SECP256K1_LIBRARIES_DIR}/gcc.lib
    )
  endif()

  target_link_libraries( ${LIB_NAME}
    PRIVATE
    "$(WindowsSDK_LibraryPath)/x64/crypt32.lib"
    "$(VC_ReferencesPath_VC_x64)/legacy_stdio_definitions.lib"
    "Userenv.lib"
  )
endif( WIN32 )


set( INSTALL_ROOT_DIR ${CMAKE_SOURCE_DIR})
install( DIRECTORY include DESTINATION ${INSTALL_ROOT_DIR}/install/include )
install( TARGETS ${LIB_NAME} 
  LIBRARY DESTINATION ${INSTALL_ROOT_DIR}/install/lib PERMISSIONS WORLD_WRITE
  ARCHIVE DESTINATION ${INSTALL_ROOT_DIR}/install/lib PERMISSIONS WORLD_WRITE
)