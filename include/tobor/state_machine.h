#ifndef TOBOR_STATE_MACHINE_H
#define TOBOR_STATE_MACHINE_H

#include <tobor/state.h>

typedef struct state_machine_s state_machine_t;

state_machine_t* state_machine_create(void);
void             state_machine_destroy(state_machine_t* state_machine);

void state_machine_start(state_machine_t* state_machine, state_tag_t* initial_state);
void state_machine_transition(state_machine_t* state_machine, state_tag_t* state);

#endif /* TOBOR_STATE_MACHINE_H */
