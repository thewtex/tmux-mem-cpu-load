cmake_minimum_required(VERSION 2.6)
if(COMMAND cmake_policy)
    cmake_policy(VERSION 2.6)
endif(COMMAND cmake_policy)

project( tmux-mem-cpu-load )

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

add_executable(tmux-mem-cpu-load tmux-mem-cpu-load.cpp)
install(TARGETS tmux-mem-cpu-load
  RUNTIME
  DESTINATION bin
  )
