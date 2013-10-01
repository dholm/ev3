#ifndef ATOMIC_H
#define ATOMIC_H

#include <inttypes.h>

#if (defined(__GNUC__) && ((__GNUC__ * 100 + __GNUC_MINOR__) >= 407))
#  define atomic_release_ptr(p, v) __atomic_store_n(*(p), (v), __ATOMIC_RELEASE)
#  define atomic_acquire_ptr(p) __atomic_load_n(*(p), __ATOMIC_ACQUIRE)

#else /* GCC < 4.7 */
#  define arm_barrier() asm volatile("" : : : "memory")
#  define atomic_release_ptr(p, v) ({ \
    arm_barrier(); \
    *p = v; })
#  define atomic_acquire_ptr(p) ({ \
    typeof(*p) val = *p; \
    arm_barrier(); \
    val; })
#endif /* GCC < 4.7 */

#endif /* ATOMIC_H */
