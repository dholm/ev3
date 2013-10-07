include_directories ("${CMAKE_CURRENT_LIST_DIR}/include")

if (${TOBOR_TARGET} MATCHES "ev3")
  add_subdirectory (ev3)
endif (${TOBOR_TARGET} MATCHES "ev3")
