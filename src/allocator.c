#include <stdlib.h>
#include <string.h>

#include "ev3/allocator.h"

static void* allocator_malloc_wrapper(void* data, size_t nmemb)
{
    allocator_tag_t* allocator_tag = (allocator_tag_t*)data;

    return malloc(allocator_tag->element_size * nmemb);
}

static void allocator_free_wrapper(void* data, void* ptr)
{
    (void)data;

    free(ptr);
}

void simple_allocator_init(allocator_tag_t* allocator_tag, size_t element_size)
{
    allocator_tag_init(allocator_tag, allocator_tag, allocator_malloc_wrapper, allocator_free_wrapper, element_size);
}
