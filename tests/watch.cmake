# A pipe must receive complete, flushed lines while the same process stays alive.
execute_process(
  COMMAND "${PROGRAM}" --watch --interval 1
  TIMEOUT 4
  RESULT_VARIABLE result
  OUTPUT_VARIABLE output
  ERROR_VARIABLE error
  )

if(NOT "${result}" MATCHES "timeout")
  message(FATAL_ERROR "Watch mode exited unexpectedly: ${result}: ${error}")
endif()

if(NOT "${error}" STREQUAL "")
  message(FATAL_ERROR "Watch mode wrote to stderr: ${error}")
endif()

string(REGEX MATCHALL "[^\n]+\n" lines "${output}")
list(LENGTH lines line_count)
if(line_count LESS 2)
  message(FATAL_ERROR "Expected repeated flushed status lines, got: ${output}")
endif()

foreach(line IN LISTS lines)
  if(NOT "${line}" MATCHES "[0-9]+/[0-9]+(MB|GB) .*% [0-9]+\\.[0-9]+")
    message(FATAL_ERROR "Incomplete or invalid status line: ${line}")
  endif()
endforeach()
