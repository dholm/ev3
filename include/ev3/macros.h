#ifndef MACROS_H
#define MACROS_H

#include <stddef.h>

#define UNUSED(x) x __attribute__((unused))

#define ATTRIBUTES(...)           __attribute__((__VA_ARGS__))
#define PACKED                    packed
#define FORMAT(fmt_idx, args_idx) format(printf, fmt_idx, args_idx)
#define NORETURN                  noreturn
#define ALIGNED(x)                aligned(x)

#define CONCAT(x, y) _CONCAT(x, y)
#define _CONCAT(x, y) x ## y

#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

#define container_of(ptr, type, member) ({ \
    const typeof(((type*) 0)->member)* __mptr = (ptr); \
    (type*)((char*) __mptr - offsetof(type, member)); })

#endif /* MACROS_H */
