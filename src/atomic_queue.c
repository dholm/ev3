#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <ev3/atomic.h>

#include "ev3/atomic_queue.h"

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

static void atomic_queue_clear(atomic_queue_t* queue)
{
    while (!atomic_queue_is_empty(queue)) {
        atomic_queue_node_tag_t* node_tag = atomic_queue_pop(queue);
        queue->node_destroy_fn(queue->node_destroy_data, node_tag);
    }
}

void atomic_queue_destroy(atomic_queue_t* queue)
{
    if (queue) {
        atomic_queue_clear(queue);

        memset(queue->null_tag, 0UL, sizeof(atomic_queue_node_tag_t));
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
        queue->node_destroy_fn(queue->node_destroy_data, tmp);
    }
}

int atomic_queue_is_empty(atomic_queue_t* queue)
{
    return atomic_acquire_ptr(&queue->divider) == atomic_acquire_ptr(&queue->last);
}

atomic_queue_node_tag_t* atomic_queue_pop(atomic_queue_t* queue)
{
    atomic_queue_node_tag_t* node_tag;

    assert(!atomic_queue_is_empty(queue));
    node_tag = atomic_acquire_ptr(&queue->divider)->next;
    atomic_release_ptr(&queue->divider, atomic_acquire_ptr(&queue->divider)->next);

    return node_tag;
}
