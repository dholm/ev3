project (tobor)

set (tobor_sources
  panic.c
  allocator.c
  array.c
  atomic_queue.c
  event_dispatcher.c
  sample.c
  slist.c)

add_library (tobor ${tobor_sources})