project (examples)

set (mrbrick_sources
  mrbrick.c)

set (examples_libs
  tobor)

add_executable (mrbrick ${mrbrick_sources})
target_link_libraries (mrbrick ${examples_libs})
