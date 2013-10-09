#include <stdlib.h>
#include <string.h>

#include "tobor/state.h"

void state_enter(state_tag_t* state)
{
    if (state->enter_state != NULL) {
        state->enter_state(state);
    }
}

void state_leave(state_tag_t* state)
{
    if (state->leave_state != NULL) {
        state->leave_state(state);
    }
}

void state_handle_event(state_tag_t* state, event_tag_t* event)
{
    if (likely(state->handle_event != NULL)) {
        state->handle_event(state, event);
    }
}
