#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <tobor/sample.h>

int sample_compare(sample_t* lhs, sample_t* rhs)
{
    assert(lhs->type == rhs->type);

    switch (lhs->type) {
        case SampleType_Int8:
            return memcmp(&lhs->value.i8, &rhs->value.i8, sizeof(lhs->value.i8));
        case SampleType_UInt8:
            return memcmp(&lhs->value.u8, &rhs->value.u8, sizeof(lhs->value.u8));
        case SampleType_Int16:
            return memcmp(&lhs->value.i16, &rhs->value.i16, sizeof(lhs->value.i16));
        case SampleType_UInt16:
            return memcmp(&lhs->value.u16, &rhs->value.u16, sizeof(lhs->value.u16));
        case SampleType_Int32:
            return memcmp(&lhs->value.i32, &rhs->value.i32, sizeof(lhs->value.i32));
        case SampleType_UInt32:
            return memcmp(&lhs->value.u32, &rhs->value.u32, sizeof(lhs->value.u32));
        case SampleType_Int64:
            return memcmp(&lhs->value.i64, &rhs->value.i64, sizeof(lhs->value.i64));
        case SampleType_UInt64:
            return memcmp(&lhs->value.u64, &rhs->value.u64, sizeof(lhs->value.u64));
        case SampleType_Float:
            return memcmp(&lhs->value.f, &rhs->value.f, sizeof(lhs->value.f));
        case SampleType_Double:
            return memcmp(&lhs->value.d, &rhs->value.d, sizeof(lhs->value.d));
        case SampleType_None:
        default:
            return 0;
    }
}

int sample_to_string(sample_t* sample, char* str, size_t size)
{
    switch (sample->type) {
        case SampleType_Int8:
            return snprintf(str, size, "%" PRIi8, sample->value.i8);
        case SampleType_UInt8:
            return snprintf(str, size, "%" PRIu8, sample->value.u8);
        case SampleType_Int16:
            return snprintf(str, size, "%" PRIi16, sample->value.i16);
        case SampleType_UInt16:
            return snprintf(str, size, "%" PRIu16, sample->value.u16);
        case SampleType_Int32:
            return snprintf(str, size, "%" PRIi32, sample->value.i32);
        case SampleType_UInt32:
            return snprintf(str, size, "%" PRIu32, sample->value.u32);
        case SampleType_Int64:
            return snprintf(str, size, "%" PRIi64, sample->value.i64);
        case SampleType_UInt64:
            return snprintf(str, size, "%" PRIu64, sample->value.u64);
        case SampleType_Float:
            return snprintf(str, size, "%f", sample->value.f);
        case SampleType_Double:
            return snprintf(str, size, "%f", sample->value.d);
        case SampleType_None:
        default:
            return snprintf(str, size, "<unknown:%d>", sample->type);
    }
}
