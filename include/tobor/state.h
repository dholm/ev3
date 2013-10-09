#ifndef TOBOR_STATE_H
#define TOBOR_STATE_H

#include <tobor/macros.h>
#include <tobor/event.h>

typedef struct state_tag_s state_tag_t;
typedef void (*state_trans_fn_t)(state_tag_t* state);
typedef void (*state_event_fn_t)(state_tag_t* state, event_tag_t* event);

#define state_decl()         state_tag_t state_tag
#define state_init(state, enter_fn, leave_fn, event_fn) \
    do { \
        (state)->enter_state = enter_fn; \
        (state)->leave_state = leave_fn; \
        (state)->handle_event = event_fn; \
    } while (0)
#define state_handle(state)  &(state)->state_tag
#define state_get(tag, type) container_of(tag, type, state_tag)

void state_enter(state_tag_t* state);
void state_leave(state_tag_t* state);
void state_handle_event(state_tag_t* state, event_tag_t* event);

struct state_tag_s {
    state_trans_fn_t enter_state;
    state_trans_fn_t leave_state;
    state_event_fn_t handle_event;
};

#endif /* TOBOR_STATE_H */
