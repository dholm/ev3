#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tobor/state_machine.h"

struct state_machine_s {
    state_tag_t* state;
};

state_machine_t* state_machine_create(void)
{
    state_machine_t* state_machine;

    state_machine = calloc(1, sizeof(state_machine_t));
    if (!state_machine) {
        return NULL;
    }

    return state_machine;
}

void state_machine_destroy(state_machine_t* state_machine)
{
    if (state_machine) {
        memset(state_machine, 0UL, sizeof(state_machine_t));
        free(state_machine);
    }
}

void state_machine_start(state_machine_t* state_machine, state_tag_t* initial_state)
{
    state_enter(initial_state);
    state_machine->state = initial_state;
}

void state_machine_transition(state_machine_t* state_machine, state_tag_t* state)
{
    if (likely(state_machine->state != NULL)) {
        state_leave(state_machine->state);
    }
    state_enter(state);
    state_machine->state = state;
}

void state_machine_handle_event(state_machine_t* state_machine, event_tag_t* event_tag)
{
    assert(state_machine->state != NULL);
    state_handle_event(state_machine->state, event_tag);
}
