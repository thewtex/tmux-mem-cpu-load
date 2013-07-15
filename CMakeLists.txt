cmake_minimum_required(VERSION 2.6)
if(COMMAND cmake_policy)
    cmake_policy(VERSION 2.6)
endif(COMMAND cmake_policy)

project( tmux-mem-cpu-load )

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE MinSizeRel CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(tmux-mem-cpu-load tmux-mem-cpu-load.cpp)
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
