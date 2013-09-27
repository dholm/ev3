#ifndef PANIC_H
#define PANIC_H

#include "macros.h"

#define panic(fmt, ...) \
    _panic("panic [%s:%d]: " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

void _panic(const char* fmt, ...) ATTRIBUTES(NORETURN, FORMAT(1, 2));

#endif /* PANIC_H */
