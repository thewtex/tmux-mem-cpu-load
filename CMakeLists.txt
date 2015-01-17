cmake_minimum_required(VERSION 2.6)
if(COMMAND cmake_policy)
    cmake_policy(VERSION 2.6)
endif(COMMAND cmake_policy)

### General Package stuff

project( tmux-mem-cpu-load )
set(VERSION_MAJOR 2)
set(VERSION_MINOR 3)
set(VERSION_PATCH 0)

# generate header file to handle version
configure_file(
   "${PROJECT_SOURCE_DIR}/version.h.in"
   "${PROJECT_SOURCE_DIR}/version.h"
   )

# set buold type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE MinSizeRel CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# detect system type
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
   message("Linux detected")
   set(METER_SOURCES "linux/memory.cc" "linux/cpu.cc" "linux/load.cc")
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
   # Mac OS X source setting will go here
   message( "Darwin detected")
   set(METER_SOURCES "osx/memory.cc" "osx/cpu.cc" "osx/load.cc")
elseif(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   # FreeBSD STUFF HERE
   message("FreeBSD detected")
   message( WARNING "FreeBSD is still experimental!" )
   set( METER_SOURCES "bsd/memory_freebsd.cc" "bsd/cpu.cc" "bsd/load.cc" )
elseif(CMAKE_SYSTEM_NAME MATCHES "OpenBSD")
   # OpenBSD Stuff Here
   message( "OpenBSD detected")
   message( FATAL_ERROR "OpenBSD is not supported! See bsd/openBSD.txt for more
   info" )
   set( METER_SOURCES "bsd/memory_openbsd.cc" "bsd/cpu.cc" "bsd/load.cc" )
else()
   message( FATAL_ERROR "Cannot be compiled on this system" )
endif()

# set common source files
SET( COMMON_SOURCES "tmux-mem-cpu-load.cpp" "graph.cc" "argParse/argParse.cc" )

# compiler flags
if (CMAKE_COMPILER_IS_GNUCC)
  execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                  OUTPUT_VARIABLE GCC_VERSION)
  if (GCC_VERSION VERSION_EQUAL 4.6)
	set( GCC_COVERAGE_COMPILE_FLAGS "-std=c++0x " )
  elseif (GCC_VERSION GREATER 4.6)
	set( GCC_COVERAGE_COMPILE_FLAGS "-std=c++11 " )
  else()
	message( FATAL_ERROR "You need gcc version >= 4.6")
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set( GCC_COVERAGE_COMPILE_FLAGS "-std=c++11 " )
else()
  message( WARNING "Untested compiler detected. You may need to set c++11
  support manually through CMakeList.txt file")
  set( GCC_COVERAGE_COMPILE_FLAGS "" )
endif()
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

# add binary tree so we find version.h
include_directories("${PROJECT_BINARY_DIR}" )

add_executable(tmux-mem-cpu-load ${COMMON_SOURCES} ${METER_SOURCES})
install(TARGETS tmux-mem-cpu-load 
   RUNTIME DESTINATION bin
)

include( CTest )
if( BUILD_TESTING )
  add_test( NAME usage
    COMMAND tmux-mem-cpu-load -h )

  add_test( NAME no_arguments
    COMMAND tmux-mem-cpu-load )

  add_test( NAME custom_interval
    COMMAND tmux-mem-cpu-load -i 3 )

  add_test( NAME colors
    COMMAND tmux-mem-cpu-load --colors )

  add_test( NAME invalid_status_interval
    COMMAND tmux-mem-cpu-load -i -1 )

  add_test( NAME invalid_graph_lines
    COMMAND tmux-mem-cpu-load --graph_lines -2 )

  set_tests_properties( usage
    invalid_status_interval
    invalid_graph_lines
    PROPERTIES WILL_FAIL TRUE )
endif()
