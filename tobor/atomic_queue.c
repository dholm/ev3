#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <tobor/atomic.h>

#include "tobor/atomic_queue.h"

struct atomic_queue_s {
    atomic_queue_node_tag_t* volatile head;
    atomic_queue_node_tag_t*          tail;
    atomic_queue_node_tag_t           stub;
};

atomic_queue_t* atomic_queue_create(void)
{
    atomic_queue_t* queue;

    queue = calloc(1, sizeof(atomic_queue_t));
    if (!queue) {
        return NULL;
    }

    queue->head = &queue->stub;
    queue->tail = &queue->stub;
    queue->stub.next = NULL;

    return queue;
}

void atomic_queue_destroy(atomic_queue_t* queue)
{
    if (queue) {
        memset(queue, 0UL, sizeof(atomic_queue_t));
        free(queue);
    }
}

int atomic_queue_is_empty(atomic_queue_t* queue)
{
    return queue->head == &queue->stub;
}

void atomic_queue_push(atomic_queue_t* queue, atomic_queue_node_tag_t* node_tag)
{
    atomic_queue_node_tag_t* prev;

    node_tag->next = NULL;
    prev = atomic_exchange_ptr(&queue->head, node_tag);
    prev->next = node_tag;
}

atomic_queue_node_tag_t* atomic_queue_pop(atomic_queue_t* queue)
{
    atomic_queue_node_tag_t* tail = queue->tail;
    atomic_queue_node_tag_t* next = tail->next;
    atomic_queue_node_tag_t* head;

    if (tail == &queue->stub) {
        if (NULL == next) {
            return NULL;
        }
        queue->tail = next;
        tail = next;
        next = next->next;
    }

    if (next) {
        queue->tail = next;
        return tail;
    }

    head = queue->head;
    if (tail != head) {
        return NULL;
    }

    atomic_queue_push(queue, &queue->stub);
    next = tail->next;
    if (next) {
        queue->tail = next;
        return tail;
    }

    return NULL;
}
