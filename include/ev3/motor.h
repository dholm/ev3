#ifndef MOTOR_H
#define MOTOR_H

#include <ev3/pwm_device.h>
#include <ev3/port.h>

typedef enum direction_e {
    Direction_Backward = -1,
    Direction_Reverse  = 0,
    Direction_Forward  = 1,
} direction_t;

typedef enum brake_e {
    Brake_Off = 0,
    Brake_On  = 1,
} brake_t;

typedef uint8_t  power_t;
typedef uint8_t  speed_t;
typedef uint32_t step_t;
typedef uint32_t time_ms_t;
typedef uint16_t turn_ratio_t;

typedef struct motor_s motor_t;

motor_t* motor_create(pwm_device_t* pwm_device, out_port_t port);
void     motor_destroy(motor_t* motor);

void motor_start(motor_t* motor);
void motor_stop(motor_t* motor, brake_t brake);

void    motor_set_direction(motor_t* motor, direction_t direction);
void    motor_set_power(motor_t* motor, power_t power);
void    motor_set_speed(motor_t* motor, speed_t speed);
speed_t motor_get_speed(motor_t* motor);

void motor_step_power(motor_t* motor, power_t power, step_t ramp_up, step_t constant, step_t ramp_down, brake_t brake);
void motor_time_power(motor_t* motor, power_t power, time_ms_t ramp_up, time_ms_t constant, time_ms_t ramp_down,
                      brake_t brake);
void motor_time_speed(motor_t* motor, speed_t power, step_t ramp_up, step_t constant, step_t ramp_down, brake_t brake);
void motor_step_sync(motor_t* motor, speed_t speed, turn_ratio_t turn_ratio, step_t steps, brake_t brake);

uint32_t motor_get_tacho_count(motor_t* motor);
void     motor_clear_tacho_count(motor_t* motor);

#endif /* MOTOR_H */
