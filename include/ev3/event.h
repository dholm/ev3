#ifndef EVENT_H
#define EVENT_H

#include <inttypes.h>

typedef int event_id_t;
typedef struct event_tag_s event_tag_t;

struct event_tag_s {
    event_id_t id;
};

#define event_tag_def()            event_tag_t event_tag
#define event_tag_init(event, id_) (event)->event_tag.id = (id_)
#define event_handle(event)        &(event)->event_tag
#define event_get(event, type)     container_of(event, type, event_tag)

#endif /* EVENT_H */
