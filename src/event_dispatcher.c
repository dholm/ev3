#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <utarray.h>

#include <ev3/event.h>
#include <ev3/atomic_queue.h>

#include "ev3/event_dispatcher.h"

struct event_dispatcher_s {
    atomic_queue_t* queue;
    UT_array*       handler_array;
};

typedef struct event_handler_s {
    event_id_t         id;
    event_handler_fn_t handler_fn;
    event_destroy_fn_t destroy_fn;
    void*              data;
} event_handler_t;

const UT_icd event_handler_icd = {sizeof(event_handler_t), NULL, NULL, NULL};

static void event_destroy_fn(event_dispatcher_t* event_dispatcher, atomic_queue_node_tag_t* node_tag)
{
    event_tag_t*     event_tag = atomic_queue_node_get(node_tag, event_tag_t);
    event_handler_t* handler   = (event_handler_t*)utarray_eltptr(event_dispatcher->handler_array, event_tag->id);

    assert(handler);
    handler->destroy_fn(handler->data, event_tag);
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

    utarray_new(event_dispatcher->handler_array, &event_handler_icd);

    return event_dispatcher;
fail:
    event_dispatcher_destroy(event_dispatcher);
    return NULL;
}

static void event_dispatcher_destroy_handlers(event_dispatcher_t* event_dispatcher)
{
    if (event_dispatcher->handler_array) {
        event_handler_t* handler = (event_handler_t*)utarray_front(event_dispatcher->handler_array);
        for (; handler; handler = (event_handler_t*)utarray_next(event_dispatcher->handler_array, handler)) {
            memset(handler, 0UL, sizeof(event_handler_t));
            free(handler);
        }
        utarray_free(event_dispatcher->handler_array);
    }
}

void event_dispatcher_destroy(event_dispatcher_t* event_dispatcher)
{
    if (event_dispatcher) {
        atomic_queue_destroy(event_dispatcher->queue);
        event_dispatcher_destroy_handlers(event_dispatcher);

        memset(event_dispatcher, 0UL, sizeof(event_dispatcher_t));
        free(event_dispatcher);
    }
}

event_id_t event_dispatcher_register_handler(event_dispatcher_t* event_dispatcher, event_handler_fn_t handler_fn,
                                             event_destroy_fn_t destroy_fn, void* data)
{
    event_handler_t* handler  = calloc(1, sizeof(event_handler_t));

    handler->handler_fn = handler_fn;
    handler->destroy_fn = destroy_fn;
    handler->data       = data;
    utarray_push_back(event_dispatcher->handler_array, handler);
    handler->id         = (event_id_t)utarray_eltidx(event_dispatcher->handler_array, handler);

    return handler->id;
}

void event_dispatcher_tick(event_dispatcher_t* event_dispatcher)
{
    if (!atomic_queue_is_empty(event_dispatcher->queue)) {
        atomic_queue_node_tag_t* node_tag  = atomic_queue_pop(event_dispatcher->queue);
        event_tag_t*             event_tag = atomic_queue_node_get(node_tag, event_tag_t);
        event_handler_t*         handler   = (event_handler_t*)utarray_eltptr(event_dispatcher->handler_array,
                                                                              event_tag->id);

        assert(handler);
        handler->handler_fn(handler->data, event_tag);
    }
}

void event_dispatcher_push(event_dispatcher_t* event_dispatcher, event_tag_t* event_tag)
{
    atomic_queue_push(event_dispatcher->queue, atomic_queue_node_tag_handle(event_tag));
}
