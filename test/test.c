#include <unistd.h>
#include <stdio.h>

#include <ev3/uart_device.h>
#include <ev3/pwm_device.h>
#include <ev3/motor.h>

const power_t SPEED = 50;

int main(void)
{
    int            i;
    uart_device_t* uart_device = uart_device_create();
    pwm_device_t*  pwm_device  = pwm_device_create();
    motor_t*       motor       = motor_create(pwm_device, OutPort_A);

    motor_set_direction(motor, Direction_Backward);
    motor_start(motor);
    motor_set_power(motor, SPEED);
    motor_set_speed(motor, SPEED);

    for (i = 0; i < 10; ++i) {
        printf("%-2d Value: %d\n", i, (int)uart_device_get_value(uart_device, InPort_1));
        sleep(1);
    }

    motor_stop(motor, Brake_Off);
    motor_destroy(motor);
    pwm_device_destroy(pwm_device);
    uart_device_destroy(uart_device);

    return 0;
}
