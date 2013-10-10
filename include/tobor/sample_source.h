#ifndef TOBOR_SAMPLE_SOURCE_H
#define TOBOR_SAMPLE_SOURCE_H

#include <tobor/macros.h>
#include <tobor/sample.h>

typedef struct sample_source_tag_s sample_source_tag_t;
typedef enum sample_source_result_e sample_source_result_t;
typedef sample_source_result_t (*sample_source_read_fn_t)(void* data, sample_value_t* out_value);

struct sample_source_tag_s {
    sample_type_t           type;
    void*                   data;
    sample_source_read_fn_t read;
};

enum sample_source_result_e {
    SampleSourceResult_Busy  = -2,
    SampleSourceResult_Error = -1,
    SampleSourceResult_Ok    = 0,
};

#define sample_source_decl()                sample_source_tag_t sample_source_tag
#define sample_source_init(sample_source, sample_type, source_data, read_fn) \
    do { \
        (sample_source)->sample_source_tag.type = sample_type, \
        (sample_source)->sample_source_tag.data = source_data, \
        (sample_source)->sample_source_tag.read = read_fn, \
    } while (0)
#define sample_source_handle(sample_source) &(sample_source)->sample_source_tag
#define sample_source_get(tag, type)        container_of(tag, type, sample_source_tag)

#define sample_source_read(sample_source, out_value) (sample_source)->read((sample_source)->data, (out_value))

#endif /* TOBOR_SAMPLE_SOURCE_H */
