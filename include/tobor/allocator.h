#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

#include <tobor/macros.h>

typedef struct allocator_tag_s allocator_tag_t;

typedef void* (*allocate_fn_t)(void* data, size_t nmemb);
typedef void  (*free_fn_t)(void* data, void* ptr);

struct allocator_tag_s {
    void*         data;
    allocate_fn_t allocate;
    free_fn_t     free;
    size_t        element_size;
};

#define allocator_tag_decl()        allocator_tag_t allocator_tag
#define allocator_handle(owner_ptr) &(owner_ptr)->allocator_tag

#define allocator_tag_init(allocator_tag, data_ptr, allocate_fn, free_fn, elem_size) \
    do { \
        (allocator_tag)->data         = data_ptr; \
        (allocator_tag)->allocate     = allocate_fn; \
        (allocator_tag)->free         = free_fn; \
        (allocator_tag)->element_size = elem_size; \
    } while (0)

#define allocator_allocate(allocator_tag, nmemb)  (allocator_tag)->allocate((allocator_tag)->data, nmemb)
#define allocator_free(allocator_tag, ptr)        (allocator_tag)->free((allocator_tag)->data, ptr)
#define allocator_get_element_size(allocator_tag) (allocator_tag)->element_size

void simple_allocator_init(allocator_tag_t* allocator_tag, size_t element_size);

#endif /* ALLOCATOR_H */
