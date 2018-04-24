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

cmake_minimum_required(VERSION 3.5)

### General Package stuff
project(tmux-mem-cpu-load)
set(tmux-mem-cpu-load_VERSION_MAJOR 3)
set(tmux-mem-cpu-load_VERSION_MINOR 4)
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
  message(FATAL_ERROR
    "Compiler ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} has no C++11 support.")
endif()

# generate header file to handle version
configure_file(
  "${PROJECT_SOURCE_DIR}/common/version.h.in" "${PROJECT_BINARY_DIR}/version.h"
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
  set(METER_SOURCES "linux/memory.cc" "linux/cpu.cc" "common/load.cc")
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  message(STATUS "Darwin detected")
  set(METER_SOURCES "osx/memory.cc" "osx/cpu.cc" "common/load.cc")
elseif(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
  message(STATUS "FreeBSD detected")
  set(METER_SOURCES "freebsd/memory.cc" "freebsd/cpu.cc" "common/load.cc")
elseif(CMAKE_SYSTEM_NAME MATCHES "OpenBSD")
  message(STATUS "OpenBSD detected")
  set(METER_SOURCES "openbsd/memory.cc" "openbsd/cpu.cc" "common/load.cc")
  if(CMAKE_SYSTEM_VERSION VERSION_LESS 5.7)
    add_definitions(-DOPENBSD_WORKAROUND=1)
  endif()
elseif(CMAKE_SYSTEM_NAME MATCHES "NetBSD")
  message(STATUS "NetBSD detected")
  set(METER_SOURCES "netbsd/memory.cc" "netbsd/cpu.cc" "common/load.cc")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
  message(STATUS "Windows detected")
  set(METER_SOURCES "windows/memory.cc" "windows/cpu.cc" "windows/load.cc")
else()
  message(FATAL_ERROR "${CMAKE_SYSTEM_NAME} Cannot be compiled on this system")
  message(FATAL_ERROR "Cannot be compiled on this system")
endif()

# set common source files
set(COMMON_SOURCES "common/main.cc" "common/memory.cc" "common/graph.cc" "common/powerline.cc")

add_executable(tmux-mem-cpu-load ${COMMON_SOURCES} ${METER_SOURCES})
# add binary tree so we find version.h
target_include_directories(tmux-mem-cpu-load PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(tmux-mem-cpu-load PUBLIC "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/common")
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  target_link_libraries(tmux-mem-cpu-load Pdh)
endif()
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

  add_test(NAME colors_short
    COMMAND tmux-mem-cpu-load -c
    )

  add_test(NAME powerline-right
    COMMAND tmux-mem-cpu-load --powerline-right
    )

  add_test(NAME powerline-left
    COMMAND tmux-mem-cpu-load --powerline-left
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

  add_test(NAME memory_mode_free_memory
    COMMAND tmux-mem-cpu-load -m 1
    )

  add_test(NAME memory_mode_used_percentage
    COMMAND tmux-mem-cpu-load -m 2
    )

  add_test(NAME averages_count_0
    COMMAND tmux-mem-cpu-load -a 0
    )

  add_test(NAME averages_count_1
    COMMAND tmux-mem-cpu-load -a 1
    )

  add_test(NAME averages_count_2
    COMMAND tmux-mem-cpu-load -a 2
    )

  add_test(NAME averages_count_3
    COMMAND tmux-mem-cpu-load -a 3
    )

  add_test(NAME cpu_mode_0
    COMMAND tmux-mem-cpu-load --cpu-mode 0
    )

  add_test(NAME cpu_mode_1
    COMMAND tmux-mem-cpu-load --cpu-mode 1
    )

  add_test(NAME cpu_mode_short_0
    COMMAND tmux-mem-cpu-load -t 0
    )

  add_test(NAME cpu_mode_short_1
    COMMAND tmux-mem-cpu-load -t 1
    )

  set_tests_properties(usage
    invalid_status_interval
    invalid_graph_lines
    old_option_specification
    PROPERTIES WILL_FAIL TRUE)
endif()
