#include "test.h"

static Suite* master_suite_create(void)
{
    return suite_create("master");
}

int main(void)
{
    int      failed;
    SRunner* runner = srunner_create(master_suite_create());

    srunner_add_suite(runner, atomic_queue_suite_create());
    srunner_add_suite(runner, event_dispatcher_suite_create());

    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return failed == 0 ? 0 : -1;
}
