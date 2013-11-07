#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include <tobor/event.h>

typedef struct event_dispatcher_s event_dispatcher_t;

typedef void (*event_handler_fn_t)(void* data, event_tag_t* event_tag);

event_dispatcher_t* event_dispatcher_create(void);
void                event_dispatcher_destroy(event_dispatcher_t* event_dispatcher);

void event_dispatcher_tick(event_dispatcher_t* event_dispatcher);

event_id_t event_dispatcher_register_handler(event_dispatcher_t* event_dispatcher, event_handler_fn_t handler_fn,
                                             void* data);

void event_dispatcher_push(event_dispatcher_t* event_dispatcher, event_tag_t* event_tag);

#endif /* EVENT_DISPATCHER_H */
