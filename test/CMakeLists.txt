project (test)

set (test_sources
  test.c
  check_allocator.c
  check_array.c
  check_atomic_queue.c
  check_event_dispatcher.c
  check_sample.c
  check_slist.c
  check_state_machine.c)

set (test_libs
  tobor
  ${CHECK_LIBRARIES})

find_package (PkgConfig)
pkg_check_modules (CHECK check>=0.9.10)
if (CHECK_FOUND)
  enable_testing ()
  link_directories (${CHECK_LIBRARY_DIRS})
  include_directories (${CHECK_INCLUDE_DIRS})

  add_executable (test ${test_sources})
  target_link_libraries (test ${test_libs})
  add_test (test ${CMAKE_CURRENT_BINARY_DIR}/test)
endif (CHECK_FOUND)
