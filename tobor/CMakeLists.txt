project (tobor)

set (tobor_sources
  panic.c
  allocator.c
  array.c
  atomic_queue.c
  event_dispatcher.c)

if (TOBOR_TARGET MATCHES "ev3")
  include_directories ("${ev3_SOURCE_DIR}/lms2012")
  set (tobor_sources
    ${tobor_sources}
    pwm_device.c
    uart_device.c
    motor.c)
endif (TOBOR_TARGET MATCHES "ev3")

add_library (tobor ${tobor_sources})