add_library( eosc_helper STATIC
  eosc_helper.cpp
)

target_include_directories( eosc_helper 
  PRIVATE
  ${Boost_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  ${SECP256K1_INCLUDE_DIR}
  ../libraries/fc/include
  ../libraries/types/include
  ../libraries/utilities/include

  PUBLIC
  include
)

target_link_libraries( eosc_helper
  PRIVATE
  ${Boost_LIBRARIES} 
  ${OPENSSL_LIBRARIES}
  fc
  eos_types
  eos_utilities
)

if( NOT EOS_ECC_IMPL STREQUAL openssl )
  target_link_libraries( eosc_helper
    PRIVATE
    ${SECP256K1_LIBRARIES}
  )
endif()

if( WIN32 )
  target_include_directories( eosc_helper
    PUBLIC
    ${OPENSSL_INCLUDE_DIR}
  )

  if( NOT EOS_ECC_IMPL STREQUAL openssl )
    target_link_libraries( eosc_helper
      PRIVATE
      ${SECP256K1_LIBRARIES_DIR}/gcc.lib
    )
  endif()

  target_link_libraries( eosc_helper
    PRIVATE
    "$(WindowsSDK_LibraryPath)/x64/crypt32.lib"
    "$(VC_ReferencesPath_VC_x64)/legacy_stdio_definitions.lib"
    "Userenv.lib"
  )
endif( WIN32 )