cmake_minimum_required (VERSION 2.8)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
include_directories ("${CMAKE_CURRENT_LIST_DIR}/include")

add_subdirectory (tobor)
add_subdirectory (drivers)
add_subdirectory (test)

if (${TOBOR_TARGET} MATCHES "ev3")
  add_subdirectory (examples)
endif (${TOBOR_TARGET} MATCHES "ev3")
