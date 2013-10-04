#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <ev3/array.h>

#include "test.h"

START_TEST(test_array_create)
{
    array_t*     array;
    const size_t element_size = 1;

    array = array_create(element_size);
    ck_assert(array != NULL);
    ck_assert(array_get(array, char) == NULL);
    ck_assert_int_eq(array_get_capacity(array), 0);

    array_destroy(array);
}
END_TEST

START_TEST(test_array_assign)
{
    array_t*       array;
    uint32_t       value        = 0xdeadbeef;
    const size_t   element_size = sizeof(typeof(value));
    const size_t   capacity     = 1;

    array = array_create(element_size);
    ck_assert(array != NULL);
    ck_assert(array_get(array, typeof(value)) == NULL);

    array_reserve(array, capacity);
    ck_assert(array_get(array, typeof(value)) != NULL);
    ck_assert_int_eq(array_get_capacity(array), capacity);

    array_at(array, 0, typeof(value)) = value;
    ck_assert_int_eq((array_get(array, typeof(value)))[0], value);

    array_destroy(array);
}
END_TEST

START_TEST(test_array_resize)
{
    array_t*       array;
    const size_t   capacity1    = 1;
    const size_t   capacity2    = 2;
    uint32_t       value1       = 0xf00fba11;
    uint32_t       value2       = 0xdeadbeef;
    const size_t   element_size = sizeof(typeof(value1));

    array = array_create(element_size);
    array_reserve(array, capacity1);
    ck_assert_int_eq(array_get_capacity(array), capacity1);
    ck_assert(array_get(array, typeof(value1)) != NULL);
    array_at(array, 0, typeof(value1)) = value1;

    array_reserve(array, capacity2);
    ck_assert_int_eq(array_get_capacity(array), capacity2);
    ck_assert_int_eq(array_at(array, 0, typeof(value1)), value1);

    array_at(array, 1, typeof(value1)) = value2;
    ck_assert_int_eq(array_at(array, 1, typeof(value1)), value2);

    array_reserve(array, capacity1);
    ck_assert_int_eq(array_get_capacity(array), capacity2);
    ck_assert_int_eq(array_at(array, 0, typeof(value1)), value1);
    ck_assert_int_eq(array_at(array, 1, typeof(value1)), value2);

    array_destroy(array);
}
END_TEST

Suite* array_suite_create(void)
{
    SUITE_PROLOG("array");

    TESTCASE("create", test_array_create);
    TESTCASE("assign", test_array_assign);
    TESTCASE("resize", test_array_resize);

    SUITE_EPILOG();
}
