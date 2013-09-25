#include <unistd.h>

#include "pwm_device.h"
#include "motor.h"

const power_t SPEED = 50;

int main(void)
{
    pwm_device_t* pwm_device = pwm_device_create();
    motor_t*      motor      = motor_create(pwm_device, OutPort_A);

    motor_start(motor);
    motor_set_power(motor, SPEED);

    sleep(2);

    motor_stop(motor, Brake_Off);

    motor_destroy(motor);
    pwm_device_destroy(pwm_device);

    return 0;
}
