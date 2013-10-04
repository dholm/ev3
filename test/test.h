#ifndef TEST_H
#define TEST_H

#include <check.h>

#define SUITE_PROLOG(name) \
    Suite* suite = suite_create(name); \
    TCase* tc

#define SUITE_EPILOG() \
    return suite

#define TESTCASE(name, fn) \
    tc = tcase_create(name); \
    tcase_add_test(tc, fn); \
    suite_add_tcase(suite, tc)

Suite* allocator_suite_create(void);
Suite* array_suite_create(void);
Suite* atomic_queue_suite_create(void);
Suite* event_dispatcher_suite_create(void);

#endif /* TEST_H */
