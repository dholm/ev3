#include "test.h"

int main(void)
{
    int      failed;
    SRunner* runner = srunner_create(allocator_suite_create());

    srunner_add_suite(runner, array_suite_create());
    srunner_add_suite(runner, slist_suite_create());
    srunner_add_suite(runner, atomic_queue_suite_create());
    srunner_add_suite(runner, event_dispatcher_suite_create());
    srunner_add_suite(runner, sample_suite_create());

    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return failed == 0 ? 0 : -1;
}
