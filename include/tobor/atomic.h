#ifndef ATOMIC_H
#define ATOMIC_H

#include <inttypes.h>

#include <tobor/cpu.h>
#include <tobor/macros.h>

#ifndef __has_builtin
#  define __has_builtin(x) 0
#endif

#define GCC_47P (defined(__GNUC__) && ((__GNUC__ * 100 + __GNUC_MINOR__) >= 407))

#if GCC_47P || __has_builtin(__atomic_store_n)
#  define atomic_store_relaxed_ptr(p, v) __atomic_store_n((p), (v), __ATOMIC_RELAXED)
#  define atomic_store_acquire_ptr(p, v) __atomic_store_n((p), (v), __ATOMIC_ACQUIRE)
#  define atomic_store_release_ptr(p, v) __atomic_store_n((p), (v), __ATOMIC_RELEASE)

#  define atomic_load_relaxed_ptr(p) __atomic_load_n((p), __ATOMIC_RELAXED)
#  define atomic_load_acquire_ptr(p) __atomic_load_n((p), __ATOMIC_ACQUIRE)
#  define atomic_load_release_ptr(p) __atomic_load_n((p), __ATOMIC_RELEASE)

#else /* GCC < 4.7 */
#  if defined(__arm__)
#    if defined(__linux__)

typedef void (*linux_barrier_fn_t)(void);
linux_barrier_fn_t arm_barrier ATTRIBUTES(WEAK) = (linux_barrier_fn_t)0xffff0fa0;

#    elif defined(ARMV5) /* !__linux__ */
#      define arm_barrier() asm volatile("" : : : "memory")
#    endif /* ARMV5 */

#      define atomic_store_relaxed_ptr(p, v) ({ *p = v; })
#      define atomic_store_acquire_ptr(p, v) ({ *p = v; arm_barrier(); })
#      define atomic_store_release_ptr(p, v) ({ arm_barrier(); *p = v; })

#      define atomic_load_relaxed_ptr(p) (*p)
#      define atomic_load_acquire_ptr(p) ({ \
    typeof(*p) val = *p; \
    arm_barrier(); \
    val; })
#      define atomic_load_release_ptr(p) ({ arm_barrier(); return *p; })

#  else /* !__arm__ */
#    error "Atomics not implemented for target architecture!"
#  endif
#endif /* GCC < 4.7 */

#endif /* ATOMIC_H */
