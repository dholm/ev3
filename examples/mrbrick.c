#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

#include <tobor/macros.h>
#include <tobor/event_dispatcher.h>
#include <tobor/sample.h>
#include <tobor/state_machine.h>

#include <ev3/uart_device.h>
#include <ev3/pwm_device.h>
#include <ev3/motor.h>

const power_t SPEED = 50;

typedef struct mrbrick_s mrbrick_t;

typedef struct sample_event_s {
    event_decl();
    sample_t sample;
} sample_event_t;

static int is_running = 1;

typedef struct {
    uart_device_t*      uart_device;
    event_dispatcher_t* event_dispatcher;
    event_id_t          event_id;
} sampler_data_t;

static void* sampler(void* data)
{
    sampler_data_t* sampler_data = (sampler_data_t*)data;

    while (is_running) {
        sample_event_t* event = calloc(1, sizeof(sample_event_t));
        event_init(event, sampler_data->event_id);
        event->sample.type     = SampleType_Int8;
        event->sample.value.i8 = uart_device_get_value(sampler_data->uart_device, InPort_1);

        event_dispatcher_push(sampler_data->event_dispatcher, event_handle(event));

        sleep(1);
    }

    return NULL;
}

typedef struct {
    state_decl();
} feed_state_t;

struct mrbrick_s {
    state_machine_t* state_machine;
    feed_state_t     feed_state;
    motor_t*         motor;
};

static void feed_state_enter(state_tag_t* state_tag)
{
    feed_state_t* feed_state = state_get(state_tag, feed_state_t);
    mrbrick_t*    mrbrick    = container_of(feed_state, mrbrick_t, feed_state);

    motor_set_direction(mrbrick->motor, Direction_Backward);
    motor_start(mrbrick->motor);
    motor_set_power(mrbrick->motor, SPEED);
    motor_set_speed(mrbrick->motor, SPEED);
}

static void feed_state_event(state_tag_t* state_tag, event_tag_t* event_tag)
{
    feed_state_t* feed_state = state_get(state_tag, feed_state_t);
    sample_event_t* sample_event = event_get(event_tag, sample_event_t);

    printf("Value: %d\n", (int)sample_event->sample.value.i8);
}

static void sample_handler(void* data, event_tag_t* event_tag)
{
    mrbrick_t*      mrbrick      = (mrbrick_t*)data;
    sample_event_t* sample_event = event_get(event_tag, sample_event_t);

    state_machine_handle_event(mrbrick->state_machine, event_tag);
    free(sample_event);
}

int main(void)
{
    int                 i;
    pwm_device_t*       pwm_device     = pwm_device_create();
    sampler_data_t      sampler_data   = {
        .uart_device      = uart_device_create(),
        .event_dispatcher = event_dispatcher_create(),
    };
    pthread_t           sampler_thread;
    mrbrick_t           mrbrick        = {
        .state_machine = state_machine_create(),
        .motor         = motor_create(pwm_device, OutPort_A),
    };

    sampler_data.event_id = event_dispatcher_register_handler(sampler_data.event_dispatcher,
                                                              sample_handler, &mrbrick);

    state_init(state_handle(&mrbrick.feed_state), feed_state_enter, NULL, feed_state_event);
    state_machine_start(mrbrick.state_machine, state_handle(&mrbrick.feed_state));

    (void)pthread_create(&sampler_thread, NULL, sampler, &sampler_data);

    for (i = 0; i < 10; ++i) {
        event_dispatcher_tick(sampler_data.event_dispatcher);
        sleep(1);
    }

    is_running = 0;
    (void)pthread_join(sampler_thread, NULL);

    state_machine_destroy(mrbrick.state_machine);
    event_dispatcher_destroy(sampler_data.event_dispatcher);
    motor_stop(mrbrick.motor, Brake_Off);
    motor_destroy(mrbrick.motor);
    pwm_device_destroy(pwm_device);
    uart_device_destroy(sampler_data.uart_device);

    return 0;
}
