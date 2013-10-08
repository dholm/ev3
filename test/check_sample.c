#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <tobor/macros.h>
#include <tobor/sample.h>

#include "test.h"

static const char* format_str(const char* fmt, ...)
{
    static char buffer[100];
    va_list     args;

    memset(&buffer[0], 0UL, sizeof(buffer));

    va_start(args, fmt);
    vsnprintf(&buffer[0], sizeof(buffer), fmt, args);
    va_end(args);

    return &buffer[0];
}

#define TEST_SAMPLE(c_type, sample_type, sample_name, fmt, val) \
    do { \
        char str[50] = {0, }; \
        const c_type CONCAT(sample_, c_type) = (val); \
        sample_t sample = { \
            .type = sample_type, \
            .value.sample_name = val, \
        }; \
        ck_assert(0 < sample_to_string(&sample, str, sizeof(str))); \
        ck_assert_str_eq(str, format_str(fmt, CONCAT(sample_, c_type))); \
    } while (0)

START_TEST(test_sample_to_string)
{
    TEST_SAMPLE(int8_t, SampleType_Int8, i8, "%d", -100);
    TEST_SAMPLE(uint8_t, SampleType_UInt8, u8, "%u", 200);
    TEST_SAMPLE(int16_t, SampleType_Int16, i16, "%d", -20000);
    TEST_SAMPLE(uint16_t, SampleType_UInt16, u16, "%d", 10000);
    TEST_SAMPLE(int32_t, SampleType_Int32, i32, "%d", -300000);
    TEST_SAMPLE(uint32_t, SampleType_UInt32, u32, "%u", 700000);
    TEST_SAMPLE(int64_t, SampleType_Int64, i64, "%" PRId64, -(0xff00000000));
    TEST_SAMPLE(uint64_t, SampleType_UInt64, u64, "%" PRIu64, 0xaf00000000);

    TEST_SAMPLE(float, SampleType_Float, f, "%f", 0.16);
    TEST_SAMPLE(double, SampleType_Double, d, "%f", (double)1/3);
}
END_TEST

Suite* sample_suite_create(void)
{
    SUITE_PROLOG("sample");

    TESTCASE("to_string", test_sample_to_string);

    SUITE_EPILOG();
}
