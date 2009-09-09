cmake_minimum_required(VERSION 2.6)
if(COMMAND cmake_policy)
    cmake_policy(VERSION 2.6)
endif(COMMAND cmake_policy)

project(screen-cpu-mem)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

add_executable(screen-cpu-usage screen-cpu-usage.cpp)
install(TARGETS screen-cpu-usage
  RUNTIME
  DESTINATION bin
  )

install(PROGRAMS screen-mem-usage
  DESTINATION bin
  )
