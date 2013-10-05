cmake_minimum_required (VERSION 2.8)
project (ev3)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
include_directories ("${PROJECT_SOURCE_DIR}/include")

add_subdirectory (tobor)

if (${TOBOR_TARGET} MATCHES "ev3")
  add_subdirectory (examples)
else (${TOBOR_TARGET} MATCHES "ev3")
  add_subdirectory (test)
endif (${TOBOR_TARGET} MATCHES "ev3")
