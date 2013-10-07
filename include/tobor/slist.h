#ifndef SLIST_H
#define SLIST_H

#include <tobor/macros.h>

typedef struct slist_s slist_t;
typedef struct slist_node_tag_s slist_node_tag_t;

slist_t* slist_create(void);
void     slist_destroy(slist_t* slist);

int slist_is_empty(slist_t* slist);

void slist_append(slist_t* slist, slist_node_tag_t* node);
void slist_prepend(slist_t* slist, slist_node_tag_t* node);

void slist_pop_front(slist_t* slist);
void slist_pop_back(slist_t* slist);
void slist_erase(slist_t* slist, slist_node_tag_t* node);

slist_node_tag_t* slist_front(slist_t* slist);
slist_node_tag_t* slist_back(slist_t* slist);
#define           slist_next(slist, node) ({ (node)->next; })

#define slist_node_decl()          slist_node_tag_t slist_node_tag
#define slist_node_tag_init(owner) ({ (owner)->slist_node_tag.next = NULL; })
#define slist_node_handle(owner)   &(owner)->slist_node_tag
#define slist_node_get(tag, type)  container_of(tag, type, slist_node_tag)

struct slist_node_tag_s {
    slist_node_tag_t* next;
};

#endif /* SLIST_H */
