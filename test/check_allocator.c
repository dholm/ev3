#include <stdlib.h>
#include <string.h>

#include <ev3/allocator.h>

#include "test.h"

START_TEST(test_simple_allocator_init)
{
    allocator_tag_t allocator_tag;
    const size_t    element_size  = 1;

    simple_allocator_init(&allocator_tag, element_size);
}
END_TEST

START_TEST(test_simple_allocator_allocate_free)
{
    allocator_tag_t allocator_tag;
    const size_t    element_size = 1;
    const size_t    count        = 2;
    void*           ptr;

    simple_allocator_init(&allocator_tag, element_size);

    ptr = allocator_allocate(&allocator_tag, count);
    ck_assert(ptr != NULL);
    memset(ptr, 0UL, element_size * count);

    allocator_free(&allocator_tag, ptr);
}
END_TEST

Suite* allocator_suite_create(void)
{
    SUITE_PROLOG("allocator");

    TESTCASE("simple_allocator_init", test_simple_allocator_init);
    TESTCASE("simple_allocator_allocate_free", test_simple_allocator_allocate_free);

    SUITE_EPILOG();
}
