#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <tobor/event.h>
#include <tobor/atomic_queue.h>
#include <tobor/array.h>

#include "tobor/event_dispatcher.h"

enum { HandlerArrayIncrements = 16 };

struct event_dispatcher_s {
    atomic_queue_t* queue;
    array_t*        handler_array;
    event_id_t      next_event_id;
};

typedef struct event_handler_s {
    event_handler_fn_t handler;
    event_destroy_fn_t destroy;
    void*              data;
} event_handler_t;

static void event_destroy_fn(event_dispatcher_t* event_dispatcher, atomic_queue_node_tag_t* node_tag)
{
    event_tag_t*     event_tag = atomic_queue_node_get(node_tag, event_tag_t);
    event_handler_t* handler;

    assert(event_id(event_tag) < array_get_capacity(event_dispatcher->handler_array));
    handler = &array_at(event_dispatcher->handler_array, event_id(event_tag), event_handler_t);
    handler->destroy(handler->data, event_tag);
}

event_dispatcher_t* event_dispatcher_create(void)
{
    event_dispatcher_t* event_dispatcher;

    event_dispatcher = calloc(1, sizeof(event_dispatcher_t));
    if (!event_dispatcher) {
        goto fail;
    }

    event_dispatcher->queue = atomic_queue_create(event_dispatcher, (atomic_queue_node_destroy_fn_t)event_destroy_fn);
    if (!event_dispatcher->queue) {
        goto fail;
    }

    event_dispatcher->handler_array = array_create(sizeof(event_handler_t));

    return event_dispatcher;
fail:
    event_dispatcher_destroy(event_dispatcher);
    return NULL;
}

void event_dispatcher_destroy(event_dispatcher_t* event_dispatcher)
{
    if (event_dispatcher) {
        atomic_queue_destroy(event_dispatcher->queue);
        array_destroy(event_dispatcher->handler_array);

        memset(event_dispatcher, 0UL, sizeof(event_dispatcher_t));
        free(event_dispatcher);
    }
}

event_id_t event_dispatcher_register_handler(event_dispatcher_t* event_dispatcher, event_handler_fn_t handler_fn,
                                             event_destroy_fn_t destroy_fn, void* data)
{
    const size_t     handler_array_capacity = array_get_capacity(event_dispatcher->handler_array);
    const event_id_t event_id               = event_dispatcher->next_event_id++;
    event_handler_t* handler;

    if (unlikely(handler_array_capacity <= event_id)) {
        array_reserve(event_dispatcher->handler_array, handler_array_capacity + HandlerArrayIncrements);
    }

    handler = &array_at(event_dispatcher->handler_array, event_id, event_handler_t);
    handler->handler = handler_fn;
    handler->destroy = destroy_fn;
    handler->data    = data;

    return event_id;
}

void event_dispatcher_tick(event_dispatcher_t* event_dispatcher)
{
    if (!atomic_queue_is_empty(event_dispatcher->queue)) {
        atomic_queue_node_tag_t* node_tag  = atomic_queue_pop(event_dispatcher->queue);
        event_tag_t*             event_tag = atomic_queue_node_get(node_tag, event_tag_t);
        event_handler_t*         handler;

        assert(event_id(event_tag) < array_get_capacity(event_dispatcher->handler_array));
        handler = &array_at(event_dispatcher->handler_array, event_id(event_tag), event_handler_t);
        handler->handler(handler->data, event_tag);
    }
}

void event_dispatcher_push(event_dispatcher_t* event_dispatcher, event_tag_t* event_tag)
{
    atomic_queue_push(event_dispatcher->queue, atomic_queue_node_handle(event_tag));
}
