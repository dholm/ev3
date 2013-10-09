#ifndef EVENT_H
#define EVENT_H

#include <tobor/macros.h>
#include <tobor/atomic_queue.h>

typedef size_t event_id_t;
typedef struct event_tag_s event_tag_t;

#define event_decl()           event_tag_t event_tag
#define event_init(event, id_) \
    do { \
        atomic_queue_node_init(&(event)->event_tag); \
        (event)->event_tag.id = (id_); \
    } while (0)
#define event_handle(event)    &(event)->event_tag
#define event_id(tag)          (tag)->id
#define event_get(tag, type)   container_of(tag, type, event_tag)

struct event_tag_s {
    atomic_queue_node_decl();
    event_id_t id;
};

#endif /* EVENT_H */
