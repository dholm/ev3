#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "lms2012.h"

const int  SPEED  = 50;

typedef enum port_e {
    Port_A = (1 << 0),
    Port_B = (1 << 1),
    Port_C = (1 << 2),
    Port_D = (1 << 3),
} port_t;

typedef struct pwm_device_s {
    int fd;
} pwm_device_t;

typedef struct motor_s {
    pwm_device_t* pwm_device;
    port_t        port;
} motor_t;

void pwm_device_destroy(pwm_device_t* pwm_device);

pwm_device_t* pwm_device_create(void)
{
    pwm_device_t* pwm_device = malloc(sizeof(*pwm_device));
    if (!pwm_device) {
        return NULL;
    }

    pwm_device->fd = open(PWM_DEVICE_NAME, O_RDWR);
    if (pwm_device->fd == -1) {
        pwm_device_destroy(pwm_device);
        return NULL;
    }
    return pwm_device;
}

void pwm_device_destroy(pwm_device_t* pwm_device)
{
    if (pwm_device) {
        close(pwm_device->fd);
        memset(pwm_device, 0UL, sizeof(*pwm_device));
        free(pwm_device);
    }
}

int pwm_device_send(pwm_device_t* pwm_device, const char* command, size_t len)
{
    return write(pwm_device->fd, command, len);
}

motor_t* motor_create(pwm_device_t* pwm_device, port_t port)
{
    motor_t* motor = malloc(sizeof(*motor));
    if (!motor) {
        return NULL;
    }

    motor->pwm_device = pwm_device;
    motor->port       = port;
    return motor;
}

void motor_destroy(motor_t* motor)
{
    if (motor) {
        memset(motor, 0UL, sizeof(*motor));
        free(motor);
    }
}

int motor_start(motor_t* motor)
{
    char motor_command[] = {
        opOUTPUT_START,
        motor->port
    };

    return pwm_device_send(motor->pwm_device, motor_command, sizeof(motor_command));
}

int motor_stop(motor_t* motor)
{
    char motor_command[] = {
        opOUTPUT_STOP,
        motor->port
    };

    return pwm_device_send(motor->pwm_device, motor_command, sizeof(motor_command));
}

int motor_power(motor_t* motor, int power)
{
    char motor_command[] = {
        opOUTPUT_POWER,
        motor->port,
        power
    };

    return pwm_device_send(motor->pwm_device, motor_command, sizeof(motor_command));
}

int main(void)
{
    pwm_device_t* pwm_device = pwm_device_create();
    motor_t*      motor      = motor_create(pwm_device, Port_A);

    motor_start(motor);
    motor_power(motor, SPEED);

    sleep(2);

    motor_stop(motor);

    motor_destroy(motor);
    pwm_device_destroy(pwm_device);

    return 0;
}
