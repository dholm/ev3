#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <tobor/macros.h>

#include "tobor/slist.h"

struct slist_s {
    slist_node_tag_t* head;
    slist_node_tag_t* tail;
};

slist_t* slist_create(void)
{
    slist_t* slist;

    slist = calloc(1, sizeof(slist_t));
    if (unlikely(slist == NULL)) {
        return NULL;
    }

    return slist;
}

void slist_destroy(slist_t* slist)
{
    if (slist) {
        memset(slist, 0UL, sizeof(slist_t));
        free(slist);
    }
}

int slist_is_empty(slist_t* slist)
{
    return slist->head == NULL;
}

void slist_append(slist_t* slist, slist_node_tag_t* node)
{
    node->next = NULL;
    if (unlikely(slist->tail == NULL)) {
        assert(slist->head == NULL);
        slist->head = slist->tail = node;
    } else {
        slist->tail = slist->tail->next = node;
    }
}

void slist_prepend(slist_t* slist, slist_node_tag_t* node)
{
    if (unlikely(slist->head == NULL)) {
        assert(slist->tail == NULL);
        slist->head = slist->tail = node;
        node->next = NULL;
    } else {
        node->next = slist->head;
        slist->head = node;
    }
}

slist_node_tag_t* slist_front(slist_t* slist)
{
    return slist->head;
}

slist_node_tag_t* slist_back(slist_t* slist)
{
    return slist->tail;
}

void slist_pop_front(slist_t* slist)
{
    if (slist->head == slist->tail) {
        slist->head = slist->tail = NULL;
    } else {
        slist->head = slist->head->next;
    }
}

void slist_pop_back(slist_t* slist)
{
    if (slist->head == slist->tail) {
        slist->head = slist->tail = NULL;
    } else {
        slist_node_tag_t* it;

        for (it = slist->head; it->next != slist->tail; it = it->next);
        it->next = NULL;
        slist->tail = it;
    }
}

void slist_erase(slist_t* slist, slist_node_tag_t* node)
{
    if (unlikely(node == slist->head)) {
        slist_pop_front(slist);
    } else if (unlikely(node == slist->tail)) {
        slist_pop_back(slist);
    } else {
        slist_node_tag_t* it;

        for (it = slist_front(slist); node != it->next; it = slist_next(slist, it));
        it->next = it->next->next;
    }
}
