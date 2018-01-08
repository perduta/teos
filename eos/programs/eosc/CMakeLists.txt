cmake_minimum_required (VERSION 3.8)
project ("Tokenika_Eosc")

set(VERSION_MAJOR 0)
set(VERSION_MINOR 5)
set(cmake_build_type debug)
set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake")
include(cotire)
include("${PROJECT_SOURCE_DIR}/CMake/config.cmake")

set(VERSION_MAJOR 0)
set(VERSION_MINOR 5)
set(CONFIG_JSON_FILE "eosc_config.json")
set(CONFIG_JSON_PATH "${CONFIG_JSON_FILE}")
set(EOSC_HELPER_INSTALL_PATH "${CMAKE_SOURCE_DIR}/../eos/install" )

set(CONFIG_JSON "\"${CONFIG_JSON_PATH}\"")
file(COPY ${CONFIG_JSON_PATH} DESTINATION "${CMAKE_BINARY_DIR}")

set(LOGGING_DIR "\"log\"")
configure_file (
   "${PROJECT_SOURCE_DIR}/eosc_config.h.in"
   "${PROJECT_SOURCE_DIR}/eosc_config.h"
   )
include_directories(${CMAKE_BINARY_DIR})

if( WIN32 )
  add_definitions( -D_WIN32_WINNT=0x0501 )
  add_compile_options( /std:c++14 )
endif( WIN32 )

####################################### DEPENDENCIES

set( Boost_USE_STATIC_LIBS ON )
set( Boost_USE_MULTITHREADED ON )
set( Boost_USE_STATIC_RUNTIME OFF )
if( WIN32 )
  set( Boost_NO_SYSTEM_PATHS ON )
  set( BOOST_INCLUDEDIR $ENV{BOOST_INCLUDEDIR})
  message( STATUS "BOOST_INCLUDEDIR: $ENV{BOOST_INCLUDEDIR}")
  set( BOOST_LIBRARYDIR $ENV{BOOST_LIBRARYDIR} )
  message( STATUS "BOOST_LIBRARYDIR: $ENV{BOOST_LIBRARYDIR}")
endif( WIN32 )
find_package(Boost 1.62 REQUIRED COMPONENTS 
  system 
  date_time 
  program_options 
  filesystem
  regex
)
if( Boost_FOUND )
  message( STATUS "Boost found.
  Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}
  Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}
  ")
else( Boost_FOUND )
  message ( STATUS "Boost not found. set environment variables:
  BOOST_LIBRARYDIR (for example, %BOOST_ROOT%/lib64-msvc-14.1)
  BOOST_INCLUDEDIR (where is boost directory, usualy %BOOST_ROOT%)
  " )
endif( Boost_FOUND )

if( NOT WIN32 AND NOT APPLE)
  find_package(Threads REQUIRED)
endif()

add_subdirectory(eosclib)

set(APPL_NAME "eosc")
add_executable(${APPL_NAME} eosc.cpp )
target_include_directories( ${APPL_NAME} 
  PRIVATE
  ${BOOST_INCLUDEDIR}
)
target_link_libraries( ${APPL_NAME}
  PRIVATE
  eosclib 
)

add_executable( "eosc_lib_test" eoec_lib_test.cpp )
target_link_libraries( "eosc_lib_test" 
  PRIVATE
  eosclib
)

cotire(${APPL_NAME})

install(TARGETS ${APPL_NAME} RUNTIME DESTINATION bin)

#######################################

add_executable( "testxxx" test.cpp )
target_include_directories( "testxxx"
  PUBLIC
  ${CMAKE_SOURCE_DIR}/eosc_helper/include
)
target_link_libraries( "testxxx"
  eosc_helper
)

#######################################

