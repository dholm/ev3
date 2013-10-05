#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <tobor/atomic.h>

#include "tobor/atomic_queue.h"

struct atomic_queue_s {
    void*                          node_destroy_data;
    atomic_queue_node_destroy_fn_t node_destroy_fn;
    atomic_queue_node_tag_t*       null_tag;
    atomic_queue_node_tag_t*       first;
    atomic_queue_node_tag_t*       divider;
    atomic_queue_node_tag_t*       last;
};

atomic_queue_t* atomic_queue_create(void* node_destroy_data, atomic_queue_node_destroy_fn_t node_destroy_fn)
{
    atomic_queue_t* queue;

    queue = calloc(1, sizeof(atomic_queue_t));
    if (!queue) {
        return NULL;
    }

    queue->node_destroy_data = node_destroy_data;
    queue->node_destroy_fn   = node_destroy_fn;
    queue->null_tag          = calloc(1, sizeof(atomic_queue_node_tag_t));
    queue->first = queue->divider = queue->last = queue->null_tag;

    return queue;
}

static void atomic_queue_destroy_node(atomic_queue_t* queue, atomic_queue_node_tag_t* node_tag)
{
    if (likely(node_tag != queue->null_tag)) {
        queue->node_destroy_fn(queue->node_destroy_data, node_tag);
    } else {
        free(queue->null_tag);
        queue->null_tag = NULL;
    }
}

static void atomic_queue_clear(atomic_queue_t* queue)
{
    while (queue->first != NULL) {
        atomic_queue_node_tag_t* node_tag = queue->first;
        queue->first = node_tag->next;
        atomic_queue_destroy_node(queue, node_tag);
    }
}

void atomic_queue_destroy(atomic_queue_t* queue)
{
    if (queue) {
        atomic_queue_clear(queue);

        free(queue->null_tag);

        memset(queue, 0UL, sizeof(atomic_queue_t));
        free(queue);
    }
}

void atomic_queue_push(atomic_queue_t* queue, atomic_queue_node_tag_t* node_tag)
{
    atomic_acquire_ptr(&queue->last)->next = node_tag;
    atomic_release_ptr(&queue->last, queue->last->next);

    while (queue->first != atomic_acquire_ptr(&queue->divider)) {
        atomic_queue_node_tag_t* tmp = queue->first;
        queue->first = queue->first->next;
        atomic_queue_destroy_node(queue, tmp);
    }
}

int atomic_queue_is_empty(atomic_queue_t* queue)
{
    return atomic_acquire_ptr(&queue->divider) == atomic_acquire_ptr(&queue->last);
}

atomic_queue_node_tag_t* atomic_queue_pop(atomic_queue_t* queue)
{
    if (!atomic_queue_is_empty(queue)) {
        atomic_queue_node_tag_t* node_tag;
        node_tag = atomic_acquire_ptr(&queue->divider)->next;
        atomic_release_ptr(&queue->divider, atomic_acquire_ptr(&queue->divider)->next);
        return node_tag;
    }

    return NULL;
}
