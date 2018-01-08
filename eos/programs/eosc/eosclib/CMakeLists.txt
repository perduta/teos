
set(LIB_NAME "eosclib")
add_library(${LIB_NAME} STATIC 
  eosc_command.cpp 
  eosc_get_commands.cpp
  eosc_wallet_commands.cpp
  subcommands.cpp
  ../eosc_test.cpp
  ../eosc
  )

target_include_directories( ${LIB_NAME} 
  PUBLIC
  ${Boost_INCLUDE_DIR}
  ${HELPER_INCLUDE}
  include
  ${CMAKE_SOURCE_DIR}/eosc_helper/include
)

target_link_libraries( ${LIB_NAME}
  PUBLIC
  eosc_helper
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

#install (TARGETS ${LIB_NAME} DESTINATION lib)
#install (FILES 
#  eosc_command.hpp 
#  eosc_get_commands.hpp 
#  eosc_wallet_commands.hpp 
#  subcommands.hpp
# DESTINATION include)