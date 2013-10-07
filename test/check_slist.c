#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <tobor/slist.h>

#include "test.h"

typedef struct sample_node_s sample_node_t;
struct sample_node_s {
    slist_node_decl();
    int i;
};

START_TEST(test_slist_create)
{
    slist_t* slist;

    slist = slist_create();
    ck_assert(slist != NULL);
    ck_assert(slist_is_empty(slist));

    slist_destroy(slist);
}
END_TEST

START_TEST(test_slist_append)
{
    int       i;
    const int count = 10;
    slist_t*  slist;

    slist = slist_create();
    ck_assert(slist != NULL);
    ck_assert(slist_is_empty(slist));

    for (i = 0; i < count; ++i) {
        sample_node_t* node = calloc(1, sizeof(sample_node_t));
        slist_node_tag_init(node);
        node->i = i;
        slist_append(slist, slist_node_handle(node));
    }

    for (i = 0; i < count; ++i) {
        sample_node_t* node = slist_node_get(slist_front(slist), sample_node_t);
        ck_assert_int_eq(node->i, i);
        slist_pop_front(slist);
        free(node);
    }

    slist_destroy(slist);
}
END_TEST

START_TEST(test_slist_prepend)
{
    int       i;
    const int count = 10;
    slist_t*  slist;

    slist = slist_create();
    ck_assert(slist != NULL);
    ck_assert(slist_is_empty(slist));

    for (i = 0; i < count; ++i) {
        sample_node_t* node = calloc(1, sizeof(sample_node_t));
        slist_node_tag_init(node);
        node->i = i;
        slist_prepend(slist, slist_node_handle(node));
    }

    for (i = 0; i < count; ++i) {
        sample_node_t* node = slist_node_get(slist_back(slist), sample_node_t);
        ck_assert_int_eq(node->i, i);
        slist_pop_back(slist);
        free(node);
    }

    slist_destroy(slist);
}
END_TEST

Suite* slist_suite_create(void)
{
    SUITE_PROLOG("slist");

    TESTCASE("create", test_slist_create);
    TESTCASE("append", test_slist_append);
    TESTCASE("prepend", test_slist_prepend);

    SUITE_EPILOG();
}
