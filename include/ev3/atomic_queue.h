#include <assert.h>

#include <ev3/macros.h>
#include <ev3/atomic.h>

#ifndef QueueName
#  error Instance QueueName not defined
#endif

#ifndef ValueType
#  error Storage ValueType not defined
#endif

#define QueueStruct struct CONCAT(QueueName, _s)
#define QueueType   CONCAT(QueueName, _t)

#define NodeStruct struct CONCAT(QueueName, _node_s)
#define NodeType   CONCAT(QueueName, _node_t)

typedef NodeStruct  NodeType;
typedef QueueStruct QueueType;

NodeStruct {
    ValueType value;
    NodeType* next;
};

QueueStruct {
    NodeType* first;
    NodeType* divider;
    NodeType* last;
};

NodeType* CONCAT(QueueName, _node_create)(void)
{
    return malloc(sizeof(NodeType));
}

void CONCAT(QueueName, _node_destroy)(NodeType* node)
{
    if (node) {
        memset(node, 0UL, sizeof(NodeType));
        free(node);
    }
}

QueueType* CONCAT(QueueName, _create)(void)
{
    QueueType* queue;

    queue = malloc(sizeof(QueueType));
    if (!queue) {
        return NULL;
    }

    queue->first = queue->divider = queue->last = CONCAT(QueueName, _node_create)();

    return queue;
}

void CONCAT(QueueName, _destroy)(QueueType* queue)
{
    if (queue) {
        while (queue->first) {
            NodeType* tmp = queue->first;
            queue->first = tmp->next;
            CONCAT(QueueName, _node_destroy)(tmp);
        }

        memset(queue, 0UL, sizeof(QueueType));
        free(queue);
    }
}

void CONCAT(QueueName, _push)(QueueType* queue, ValueType value)
{
    atomic_acquire_ptr(&queue->last)->next = CONCAT(QueueName, _node_create)();
    atomic_acquire_ptr(&queue->last)->next->value = value;
    atomic_release_ptr(&queue->last, queue->last->next);

    while (queue->first != atomic_acquire_ptr(&queue->divider)) {
        NodeType* tmp = queue->first;
        queue->first = queue->first->next;
        CONCAT(QueueName, _node_destroy(tmp));
    }
}

int CONCAT(QueueName, _is_empty)(QueueType* queue)
{
    return atomic_acquire_ptr(&queue->divider) == atomic_acquire_ptr(&queue->last);
}

ValueType CONCAT(QueueName, _pop)(QueueType* queue)
{
    ValueType value;

    assert(!CONCAT(QueueName, _is_empty)(queue));
    value = atomic_acquire_ptr(&queue->divider)->next->value;
    atomic_release_ptr(&queue->divider, atomic_acquire_ptr(&queue->divider)->next);

    return value;
}

#undef QueueStruct
#undef QueueType

#undef NodeStruct
#undef NodeType

#undef ValueType
#undef QueueName
