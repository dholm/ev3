project (ev3-driver)

set (ev3-driver_sources
  pwm_device.c
  uart_device.c
  motor.c)

add_library (ev3-driver ${ev3-driver_sources})