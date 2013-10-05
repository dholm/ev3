#include <stdlib.h>
#include <string.h>

#include <ev3/macros.h>

#include "ev3/array.h"

array_t* array_create(size_t element_size)
{
    array_t* array;

    array = calloc(1, sizeof(array_t));
    if (!array) {
        return NULL;
    }

    simple_allocator_init(allocator_handle(array), element_size);

    return array;
}

void array_destroy(array_t* array)
{
    if (array) {
        allocator_free(allocator_handle(array), array->data);

        memset(array, 0UL, sizeof(array_t));
        free(array);
    }
}

void array_reserve(array_t* array, size_t n)
{
    if (likely(array->capacity < n)) {
        void*  data;
        size_t element_size = array_get_element_size(array);

        data = allocator_allocate(allocator_handle(array), element_size * n);
        if (likely(data != NULL)) {
            memcpy(data, array->data, element_size * array->capacity);
            allocator_free(allocator_handle(array), array->data);
            array->data     = data;
            array->capacity = n;
        }
    }
}
