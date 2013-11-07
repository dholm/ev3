#include <stdlib.h>
#include <string.h>

#include <tobor/event_dispatcher.h>

#include "test.h"

typedef struct {
    event_decl();
    int i;
} test_event_t;

static void test_event_handler(void* data, event_tag_t* event_tag)
{
    test_event_t* retval     = (test_event_t*)data;
    test_event_t* test_event;

    ck_assert(data != NULL);
    ck_assert(event_tag != NULL);

    test_event = event_get(event_tag, test_event_t);
    ck_assert(test_event != NULL);

    *retval = *test_event;
    free(test_event);
}

START_TEST(test_event_dispatcher_create)
{
    event_dispatcher_t* event_dispatcher;

    event_dispatcher = event_dispatcher_create();
    ck_assert(event_dispatcher != NULL);

    event_dispatcher_destroy(event_dispatcher);
}
END_TEST

START_TEST(test_event_dispatcher_push)
{
    event_dispatcher_t* event_dispatcher;
    event_id_t          test_event_id;
    test_event_t        retval;
    test_event_t*       test_event;
    const int           i                = 1;

    event_dispatcher = event_dispatcher_create();
    test_event_id = event_dispatcher_register_handler(event_dispatcher, test_event_handler, &retval);

    test_event = calloc(1, sizeof(test_event_t));
    event_init(test_event, test_event_id);
    test_event->i = i;
    event_dispatcher_push(event_dispatcher, event_handle(test_event));

    event_dispatcher_tick(event_dispatcher);
    ck_assert_int_eq(retval.i, i);

    event_dispatcher_destroy(event_dispatcher);
}
END_TEST

Suite* event_dispatcher_suite_create(void)
{
    SUITE_PROLOG("event_dispatcher");

    TESTCASE("create", test_event_dispatcher_create);
    TESTCASE("push", test_event_dispatcher_push);

    SUITE_EPILOG();
}
