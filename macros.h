#ifndef TOOLCHAIN_H
#define TOOLCHAIN_H

#include <stddef.h>

#define UNUSED __attribute__((unused))
#define PACKED __attribute__((packed))

#define container_of(ptr, type, member) ({ \
    const typeof(((type*) 0)->member)* __mptr = (ptr); \
    (type*)((char*) __mptr - offsetof(type, member)); })

#endif /* TOOLCHAIN_H */
