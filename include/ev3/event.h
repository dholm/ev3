#ifndef EVENT_H
#define EVENT_H

#include <inttypes.h>

#include <ev3/atomic_queue.h>

typedef int event_id_t;
typedef struct event_tag_s event_tag_t;

struct event_tag_s {
    atomic_queue_node_tag_decl();
    event_id_t id;
};

#define event_tag_decl()           event_tag_t event_tag
#define event_tag_init(event, id_) \
    do { \
        atomic_queue_node_tag_init(&(event)->event_tag); \
        (event)->event_tag.id = (id_); \
    } while (0)
#define event_handle(event)        &(event)->event_tag
#define event_id(tag)              (tag)->id
#define event_get(tag, type)       container_of(tag, type, event_tag)

#endif /* EVENT_H */
