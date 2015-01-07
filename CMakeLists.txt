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



if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE MinSizeRel CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# detect system type
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
   message( "Linux detected")
   SET( METER_SOURCES "linux/memory.cc" "linux/cpu.cc" "linux/load.cc" )
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Darwin")
   # Mac OS X source setting will go here
   message( "Darwin detected")
   SET( METER_SOURCES "osx/memory.cc" "osx/cpu.cc" "osx/load.cc" )
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   # FreeBSD STUFF HERE
   message( "FreeBSD detected")
   message( FATAL_ERROR "Free BSD is not supported yet" )
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "OpenBSD")
   # OpenBSD Stuff Here
   message( "OpenBSD detected")
   message( FATAL_ERROR "OpenBSD is not supported yet" )
ELSE()
   message( FATAL_ERROR "Cannot be compiled on this system" )
endif()

# Search for boost
# will be needed later
#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_SUE_STATIC_RUTIME OFF)
#find_package(Boost 1.52.0 REQUIRED COMPONENTS filesystem program_options system)

# compiler flags
SET( GCC_COVERAGE_COMPILE_FLAGS "-std=c++11 " )
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

# add binary tree so we find version.h
#include_directories("${PROJECT_BINARY_DIR}" ${Boost_INCLUDE_DIRS})
include_directories("${PROJECT_BINARY_DIR}" ${Boost_INCLUDE_DIRS})

add_executable(tmux-mem-cpu-load tmux-mem-cpu-load.cpp graph.cc ${METER_SOURCES})
#target_link_libraries(tmux-mem-cpu-load ${Boost_LIBRARIES})
install(TARGETS tmux-mem-cpu-load 
   RUNTIME DESTINATION bin
)

include( CTest )
if( BUILD_TESTING )
  add_test( NAME usage
    COMMAND tmux-mem-cpu-load -h )

  add_test( NAME no_arguments
    COMMAND tmux-mem-cpu-load )

  add_test( NAME colors
    COMMAND tmux-mem-cpu-load --colors )

  add_test( NAME arguments
    COMMAND tmux-mem-cpu-load --colors 1 4 )

  add_test( NAME invalid_status_interval
    COMMAND tmux-mem-cpu-load -1 4 )

  add_test( NAME invalid_graph_lines
    COMMAND tmux-mem-cpu-load 1 -4 )

  set_tests_properties( usage
    invalid_status_interval
    invalid_graph_lines
    PROPERTIES WILL_FAIL TRUE )
endif()
