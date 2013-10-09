#include <stdlib.h>
#include <string.h>

#include <tobor/state_machine.h>

#include "test.h"

typedef struct test_state_s test_state_t;
struct test_state_s {
    state_decl();
    int entered;
    int left;
};

static void test_state_enter(state_tag_t* state_tag)
{
    test_state_t* test_state = state_get(state_tag, test_state_t);
    ++test_state->entered;
}

static void test_state_leave(state_tag_t* state_tag)
{
    test_state_t* test_state = state_get(state_tag, test_state_t);
    ++test_state->left;
}

START_TEST(test_state_machine_create)
{
    state_machine_t* state_machine;

    state_machine = state_machine_create();
    ck_assert(state_machine != NULL);

    state_machine_destroy(state_machine);
}
END_TEST

START_TEST(test_state_machine_start)
{
    state_machine_t* state_machine;
    test_state_t     test_state;

    state_machine = state_machine_create();

    memset(&test_state, 0UL, sizeof(test_state_t));
    state_init(state_handle(&test_state), NULL, NULL, NULL);
    state_machine_start(state_machine, state_handle(&test_state));
    ck_assert_int_eq(test_state.entered, 0);

    state_init(state_handle(&test_state), test_state_enter, NULL, NULL);
    state_machine_start(state_machine, state_handle(&test_state));
    ck_assert_int_eq(test_state.entered, 1);

    state_machine_destroy(state_machine);
}
END_TEST

START_TEST(test_state_machine_transition)
{
    state_machine_t* state_machine;
    test_state_t     test_state[2];

    state_machine = state_machine_create();

    memset(&test_state[0], 0UL, sizeof(test_state));
    state_init(state_handle(&test_state[0]), NULL, NULL, NULL);
    state_init(state_handle(&test_state[1]), NULL, NULL, NULL);
    state_machine_start(state_machine, state_handle(&test_state[0]));
    state_machine_transition(state_machine, state_handle(&test_state[1]));
    ck_assert_int_eq(test_state[0].entered, 0);
    ck_assert_int_eq(test_state[0].left, 0);
    ck_assert_int_eq(test_state[1].entered, 0);
    ck_assert_int_eq(test_state[1].left, 0);

    state_init(state_handle(&test_state[0]), test_state_enter, test_state_leave, NULL);
    state_init(state_handle(&test_state[1]), test_state_enter, test_state_leave, NULL);
    state_machine_start(state_machine, state_handle(&test_state[0]));
    state_machine_transition(state_machine, state_handle(&test_state[1]));
    ck_assert_int_eq(test_state[0].entered, 1);
    ck_assert_int_eq(test_state[0].left, 1);
    ck_assert_int_eq(test_state[1].entered, 1);
    ck_assert_int_eq(test_state[1].left, 0);

    state_machine_destroy(state_machine);
}
END_TEST

Suite* state_machine_suite_create(void)
{
    SUITE_PROLOG("state_machine");

    TESTCASE("create", test_state_machine_create);
    TESTCASE("start", test_state_machine_start);
    TESTCASE("transition", test_state_machine_transition);

    SUITE_EPILOG();
}
