#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <lms2012/lms2012.h>

#include <tobor/macros.h>
#include <tobor/panic.h>

#include <ev3/motor.h>

struct motor_s {
    pwm_device_t* pwm_device;
    out_port_t    port;
    int           fd;
    MOTORDATA*    data;
};

static void motor_reset(motor_t* motor);
static int  motor_mmap(motor_t* motor);
static void motor_munmap(motor_t* motor);

motor_t* motor_create(pwm_device_t* pwm_device, out_port_t port)
{
    motor_t* motor = NULL;

    motor = calloc(1, sizeof(motor_t));
    if (unlikely(!motor)) {
        panic("Memory allocation failed!");
    }

    motor->pwm_device = pwm_device;
    motor->port       = port;

    motor->fd         = open(MOTOR_DEVICE_NAME, O_RDWR | O_SYNC);
    if (unlikely(motor->fd == -1)) {
        panic("Unable to open %s!", MOTOR_DEVICE_NAME);
    }

    if (unlikely(!motor_mmap(motor))) {
        panic("Failed to map motor device!");
    }

    motor_reset(motor);

    return motor;
}

void motor_destroy(motor_t* motor)
{
    if (motor) {
        motor_munmap(motor);

        memset(motor, 0UL, sizeof(*motor));
        free(motor);
    }
}

static int motor_mmap(motor_t* motor)
{
    int   prot   = PROT_READ | PROT_WRITE;
    int   flags  = MAP_FILE | MAP_SHARED;
    int   idx    = __builtin_ctz((unsigned int)motor->port);
    off_t offset = idx * sizeof(MOTORDATA);

    motor->data = (MOTORDATA*)mmap(0, sizeof(MOTORDATA), prot, flags, motor->fd, offset);
    if (motor->data == MAP_FAILED) {
        motor->data = NULL;
        return 0;
    }
    return 1;
}

static void motor_munmap(motor_t* motor)
{
    (void)munmap(motor->data, sizeof(MOTORDATA));
}

typedef struct motor_cmd_s {
    DATA8 cmd;
    DATA8 nos;
    DATA8 arg;
} ATTRIBUTES(PACKED) motor_cmd_t;

#define motor_cmd(motor, command, argument) { \
    .cmd = (DATA8)command, \
    .nos = (DATA8)(motor)->port, \
    .arg = (DATA8)argument }

#define motor_send_cmd(motor, type, cmd_ptr) ({ \
    const type* _cmd = container_of(cmd_ptr, type, hdr); \
    pwm_device_send((motor)->pwm_device, (const uint8_t*)_cmd, sizeof(type)); })

#define motor_send_cmd_raw(motor, ...) ({ \
    uint8_t command[] = {__VA_ARGS__}; \
    pwm_device_send((motor)->pwm_device, (const uint8_t*)&command[0], sizeof(command)); })

static void motor_reset(motor_t* motor)
{
    (void)motor_send_cmd_raw(motor, opOUTPUT_RESET);
}

void motor_start(motor_t* motor)
{
    (void)motor_send_cmd_raw(motor, opOUTPUT_START);
}

void motor_stop(motor_t* motor, brake_t brake)
{
    (void)motor_send_cmd_raw(motor, opOUTPUT_STOP, motor->port, (DATA8) brake);
}

void motor_set_direction(motor_t* motor, direction_t direction)
{
    (void)motor_send_cmd_raw(motor, opOUTPUT_POLARITY, motor->port, (DATA8) direction);
}

void motor_set_power(motor_t* motor, power_t power)
{
    (void)motor_send_cmd_raw(motor, opOUTPUT_POWER, motor->port, (DATA8) power);
}

void motor_set_speed(motor_t* motor, speed_t speed)
{
    (void)motor_send_cmd_raw(motor, opOUTPUT_SPEED, motor->port, (DATA8) speed);
}

speed_t motor_get_speed(motor_t* motor)
{
    return (speed_t)motor->data->Speed;
}

typedef struct motor_step_cmd_s {
    motor_cmd_t hdr;
    DATA32      ramp_up;
    DATA32      constant;
    DATA32      ramp_down;
    DATA8       brake;
} ATTRIBUTES(PACKED) motor_step_cmd_t;

#define motor_step_cmd(motor, command, argument, ramp_up, constant, ramp_down, brake) { \
    .hdr       = motor_cmd(motor, command, argument), \
    .ramp_up   = (DATA32)ramp_up, \
    .constant  = (DATA32)constant, \
    .ramp_down = (DATA32)ramp_down, \
    .brake     = (DATA8)brake }

void motor_step_power(motor_t* motor, power_t power, step_t ramp_up, step_t constant, step_t ramp_down, brake_t brake)
{
    motor_step_cmd_t cmd = motor_step_cmd(motor, opOUTPUT_STEP_POWER, power, ramp_up, constant, ramp_down, brake);
    (void)motor_send_cmd(motor, motor_step_cmd_t, &cmd.hdr);
}

void motor_time_power(motor_t* motor, power_t power, time_ms_t ramp_up, time_ms_t constant, time_ms_t ramp_down,
                      brake_t brake)
{
    motor_step_cmd_t cmd = motor_step_cmd(motor, opOUTPUT_TIME_POWER, power, ramp_up, constant, ramp_down, brake);
    (void)motor_send_cmd(motor, motor_step_cmd_t, &cmd.hdr);
}

void motor_time_speed(motor_t* motor, speed_t speed, time_ms_t ramp_up, time_ms_t constant, time_ms_t ramp_down,
                      brake_t brake)
{
    motor_step_cmd_t cmd = motor_step_cmd(motor, opOUTPUT_TIME_SPEED, speed, ramp_up, constant, ramp_down, brake);
    (void)motor_send_cmd(motor, motor_step_cmd_t, &cmd.hdr);
}

void motor_step_sync(motor_t* motor, speed_t speed, turn_ratio_t turn_ratio, step_t steps, brake_t brake)
{
    struct motor_step_sync_cmd_s {
        motor_cmd_t hdr;
        DATA16      turn_ratio;
        DATA32      steps;
        DATA8       brake;
    } ATTRIBUTES(PACKED) cmd = {
        .hdr        = motor_cmd(motor, opOUTPUT_STEP_SYNC, speed),
        .turn_ratio = (DATA16)turn_ratio,
        .steps      = (DATA32)steps,
        .brake      = (DATA8)brake,
    };
    (void) motor_send_cmd(motor, struct motor_step_sync_cmd_s, &cmd.hdr);
}

uint32_t motor_get_tacho_count(motor_t* motor)
{
    return (uint32_t)motor->data->TachoSensor;
}

void motor_clear_tacho_count(motor_t* motor)
{
    (void)motor_send_cmd_raw(motor, opOUTPUT_CLR_COUNT);
    motor->data->TachoSensor = 0;
}
