project (ev3-driver)

include_directories ("${ev3-driver_SOURCE_DIR}/lms2012")
set (ev3-driver_sources
  pwm_device.c
  uart_device.c
  motor.c)

add_library (ev3-driver ${ev3-driver_sources})