#ifndef ARRAY_H
#define ARRAY_H

#include <ev3/allocator.h>

struct array_s {
    allocator_tag_decl();
    size_t capacity;
    void*  data;
};

typedef struct array_s array_t;

array_t* array_create(size_t element_size);
void     array_destroy(array_t* array);

void array_reserve(array_t* array, size_t n);

#define array_get_capacity(array)     (array)->capacity
#define array_get_element_size(array) allocator_get_element_size(allocator_handle(array))
#define array_get(array, type)        ((type*)(array)->data)
#define array_at(array, pos, type)    array_get(array, type)[pos]

#endif /* ARRAY_H */
