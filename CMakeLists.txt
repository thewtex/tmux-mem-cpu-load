# vim: tabstop=2 shiftwidth=2 expandtab textwidth=80 linebreak wrap
#
# Copyright 2012 Matthew McCormick
# Copyright 2015 Pawel 'l0ner' Soltys
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 2.6)
if(COMMAND cmake_policy)
  cmake_policy(VERSION 2.6)
endif(COMMAND cmake_policy)

### General Package stuff
project( tmux-mem-cpu-load )
set(tmux-mem-cpu-load_VERSION_MAJOR 2)
set(tmux-mem-cpu-load_VERSION_MINOR 3)
set(tmux-mem-cpu-load_VERSION_PATCH 0)
#Compute full version string
set(tmux-mem-cpu-load_VERSION
  ${tmux-mem-cpu-load_VERSION_MAJOR}.${tmux-mem-cpu-load_VERSION_MINOR}.${tmux-mem-cpu-load_VERSION_PATCH})

# Check whether we have support for c++11 in compiler and fail if we don't
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(FATAL_ERROR "
Compiler ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} has no C++11 support.")
endif()

# generate header file to handle version
configure_file(
  "${PROJECT_SOURCE_DIR}/version.h.in" "${PROJECT_SOURCE_DIR}/version.h"
  )

# set build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE MinSizeRel CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# detect system type
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  message(STATUS "Linux detected")
  set(METER_SOURCES "linux/memory.cc" "linux/cpu.cc" "linux/load.cc")
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  message(STATUS "Darwin detected")
  set(METER_SOURCES "osx/memory.cc" "osx/cpu.cc" "osx/load.cc")
elseif(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
  message("FreeBSD detected")
  message(WARNING "FreeBSD is still experimental!")
  set(METER_SOURCES "bsd/memory_freebsd.cc" "bsd/cpu.cc" "bsd/load.cc")
elseif(CMAKE_SYSTEM_NAME MATCHES "OpenBSD")
  message(STATUS "OpenBSD detected")
  message(FATAL_ERROR
    "OpenBSD is not supported! See bsd/openBSD.txt for more info")
  set(METER_SOURCES "bsd/memory_openbsd.cc" "bsd/cpu.cc" "bsd/load.cc")
else()
  message(FATAL_ERROR "Cannot be compiled on this system" )
endif()

# set common source files
set(COMMON_SOURCES "tmux-mem-cpu-load.cc" "graph.cc")

# add binary tree so we find version.h
include_directories("${PROJECT_BINARY_DIR}")

add_executable(tmux-mem-cpu-load ${COMMON_SOURCES} ${METER_SOURCES})
install(TARGETS tmux-mem-cpu-load RUNTIME DESTINATION bin)

include(CTest)
if(BUILD_TESTING)
  add_test(NAME usage
    COMMAND tmux-mem-cpu-load -h
    )

  add_test(NAME no_arguments
    COMMAND tmux-mem-cpu-load
    )

  add_test(NAME custom_interval
    COMMAND tmux-mem-cpu-load -i 3
    )

  add_test(NAME no_cpu_graph
    COMMAND tmux-mem-cpu-load -g 0
    )

  add_test(NAME colors
    COMMAND tmux-mem-cpu-load --colors
    )

  add_test(NAME invalid_status_interval
    COMMAND tmux-mem-cpu-load -i -1
    )

  add_test(NAME invalid_graph_lines
    COMMAND tmux-mem-cpu-load --graph_lines -2
    )

  add_test(NAME old_option_specification
    COMMAND tmux-mem-cpu-load 2 8
    )

  set_tests_properties(usage
    invalid_status_interval
    invalid_graph_lines
    old_option_specification
    PROPERTIES WILL_FAIL TRUE)
endif()
