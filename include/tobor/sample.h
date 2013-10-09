#ifndef TOBOR_SAMPLE_H
#define TOBOR_SAMPLE_H

#include <stddef.h>
#include <inttypes.h>

typedef struct sample_s sample_t;
typedef enum sample_type_e sample_type_t;

enum sample_type_e {
    SampleType_None,
    SampleType_Int8,
    SampleType_UInt8,
    SampleType_Int16,
    SampleType_UInt16,
    SampleType_Int32,
    SampleType_UInt32,
    SampleType_Int64,
    SampleType_UInt64,
    SampleType_Float,
    SampleType_Double,
};

struct sample_s {
    sample_type_t type;
    union {
        int8_t   i8;
        uint8_t  u8;
        int16_t  i16;
        uint16_t u16;
        int32_t  i32;
        uint32_t u32;
        int64_t  i64;
        uint64_t u64;
        float    f;
        double   d;
    } value;
};

int sample_compare(sample_t* lhs, sample_t* rhs);
int sample_to_string(sample_t* sample, char* str, size_t size);

#endif /* TOBOR_SAMPLE_H */
