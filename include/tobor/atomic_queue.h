#ifndef ATOMIC_QUEUE_H
#define ATOMIC_QUEUE_H

#include <tobor/macros.h>

typedef struct atomic_queue_s atomic_queue_t;
typedef struct atomic_queue_node_tag_s atomic_queue_node_tag_t;

#define atomic_queue_node_decl()         atomic_queue_node_tag_t atomic_queue_node_tag
#define atomic_queue_node_init(node)     (node)->atomic_queue_node_tag.next = NULL
#define atomic_queue_node_handle(node)   &(node)->atomic_queue_node_tag
#define atomic_queue_node_get(tag, type) container_of(tag, type, atomic_queue_node_tag)

atomic_queue_t* atomic_queue_create(void);
void            atomic_queue_destroy(atomic_queue_t* queue);

int atomic_queue_is_empty(atomic_queue_t* queue);

void                     atomic_queue_push(atomic_queue_t* queue, atomic_queue_node_tag_t* node_tag);
atomic_queue_node_tag_t* atomic_queue_pop(atomic_queue_t* queue);

struct atomic_queue_node_tag_s {
    atomic_queue_node_tag_t* volatile next;
};

#endif /* ATOMIC_QUEUE_H */
