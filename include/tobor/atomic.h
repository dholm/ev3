#ifndef ATOMIC_H
#define ATOMIC_H

#include <inttypes.h>

#ifndef __has_builtin
#  define __has_builtin(x) 0
#endif

#define GCC_47P (defined(__GNUC__) && ((__GNUC__ * 100 + __GNUC_MINOR__) >= 407))

#if GCC_47P || __has_builtin(__atomic_store_n)
#  define atomic_release_ptr(p, v) __atomic_store_n((p), (v), __ATOMIC_RELEASE)
#  define atomic_acquire_ptr(p) __atomic_load_n((p), __ATOMIC_ACQUIRE)

#else /* GCC < 4.7 */
#  if defined(__arm__)
#    define arm_barrier() asm volatile("" : : : "memory")
#    define atomic_release_ptr(p, v) ({ \
    arm_barrier(); \
    *p = v; })
#    define atomic_acquire_ptr(p) ({ \
    typeof(*p) val = *p; \
    arm_barrier(); \
    val; })
#  else
#    error "Atomics not implemented for target architecture!"
#  endif
#endif /* GCC < 4.7 */

#endif /* ATOMIC_H */
