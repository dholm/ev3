#include <stdlib.h>
#include <string.h>

#include <ev3/atomic_queue.h>

#include "test.h"

typedef struct {
    atomic_queue_node_tag_decl();
    int i;
} test_node_t;

static void test_node_destroy(void* data, atomic_queue_node_tag_t* node_tag)
{
    test_node_t* test_node;

    ck_assert(data == NULL);
    ck_assert(node_tag != NULL);

    test_node = atomic_queue_node_get(node_tag, test_node_t);
    ck_assert(test_node != NULL);

    memset(test_node, 0UL, sizeof(test_node_t));
    free(test_node);
}

START_TEST(test_atomic_queue_create)
{
    atomic_queue_t* queue;

    queue = atomic_queue_create(NULL, test_node_destroy);
    ck_assert(queue != NULL);

    ck_assert(atomic_queue_is_empty(queue));

    atomic_queue_destroy(queue);
}
END_TEST

START_TEST(test_atomic_queue_push_pop)
{
    atomic_queue_node_tag_t* node_tag;
    atomic_queue_t*          queue;
    test_node_t*             node;
    const int                i        = 1;

    queue = atomic_queue_create(NULL, test_node_destroy);
    ck_assert(queue != NULL);

    node = calloc(1, sizeof(test_node_t));
    atomic_queue_node_tag_init(node);
    node->i = i;
    atomic_queue_push(queue, atomic_queue_node_tag_handle(node));
    ck_assert(!atomic_queue_is_empty(queue));

    node_tag = atomic_queue_pop(queue);
    ck_assert(node_tag != NULL);
    ck_assert(atomic_queue_is_empty(queue));

    node = atomic_queue_node_get(node_tag, test_node_t);
    ck_assert(node != NULL);
    ck_assert_int_eq(node->i, i);

    atomic_queue_destroy(queue);
}
END_TEST

START_TEST(test_atomic_queue_push_pop_many)
{
    atomic_queue_node_tag_t* node_tag;
    atomic_queue_t*          queue;
    test_node_t*             node;
    const int                count    = 10;
    int                      i;
    int                      j;

    queue = atomic_queue_create(NULL, test_node_destroy);
    ck_assert(queue != NULL);

    for (i = 0; i < count; i += 2) {
        for (j = 0; j < 2; ++j) {
            node = calloc(1, sizeof(test_node_t));
            atomic_queue_node_tag_init(node);
            node->i = i + j;
            atomic_queue_push(queue, atomic_queue_node_tag_handle(node));
        }

        for (j = 0; j < 2; ++j) {
            ck_assert(!atomic_queue_is_empty(queue));
            node_tag = atomic_queue_pop(queue);
            ck_assert(node_tag != NULL);

            node = atomic_queue_node_get(node_tag, test_node_t);
            ck_assert(node != NULL);
            ck_assert_int_eq(node->i, i + j);
        }
    }
    ck_assert(atomic_queue_is_empty(queue));

    atomic_queue_destroy(queue);
}
END_TEST

Suite* atomic_queue_suite_create(void)
{
    SUITE_PROLOG("atomic_queue");

    TESTCASE("create", test_atomic_queue_create);
    TESTCASE("push/pop", test_atomic_queue_push_pop);
    TESTCASE("push/pop many", test_atomic_queue_push_pop_many);

    SUITE_EPILOG();
}
