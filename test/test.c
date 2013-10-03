#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <ev3/macros.h>
#include <ev3/uart_device.h>
#include <ev3/pwm_device.h>
#include <ev3/motor.h>
#include <ev3/event_dispatcher.h>

const power_t SPEED = 50;

typedef struct sample_event_s {
    event_tag_def();
    uint8_t value;
} sample_event_t;

static void sample_event_destroy(void* data, sample_event_t* sample_event)
{
    (void)data;

    memset(sample_event, 0UL, sizeof(sample_event_t));
    free(sample_event);
}

static void sample_handler(void* data, event_tag_t* event_tag)
{
    sample_event_t* sample_event = event_get(event_tag, sample_event_t);
    (void)data;

    printf("Value: %d\n", (int)sample_event->value);
    free(sample_event);
}

int main(void)
{
    int                 i;
    uart_device_t*      uart_device      = uart_device_create();
    pwm_device_t*       pwm_device       = pwm_device_create();
    motor_t*            motor            = motor_create(pwm_device, OutPort_A);
    event_dispatcher_t* event_dispatcher = event_dispatcher_create();
    event_id_t          sample_ev_id;

    sample_ev_id = event_dispatcher_register_handler(event_dispatcher, sample_handler,
                                                     (event_destroy_fn_t)sample_event_destroy, NULL);

    motor_set_direction(motor, Direction_Backward);
    motor_start(motor);
    motor_set_power(motor, SPEED);
    motor_set_speed(motor, SPEED);

    for (i = 0; i < 10; ++i) {
        sample_event_t* event = calloc(1, sizeof(sample_event_t));
        event_tag_init(event, sample_ev_id);
        event->value = uart_device_get_value(uart_device, InPort_1);
        event_dispatcher_push(event_dispatcher, event_handle(event));
        event_dispatcher_tick(event_dispatcher);
        sleep(1);
    }

    event_dispatcher_destroy(event_dispatcher);
    motor_stop(motor, Brake_Off);
    motor_destroy(motor);
    pwm_device_destroy(pwm_device);
    uart_device_destroy(uart_device);

    return 0;
}
