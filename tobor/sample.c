#include <stdio.h>

#include <tobor/sample.h>

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
