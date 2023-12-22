/*
 * Copyright 2022 Google LLC
 * SPDX-License-Identifier: MIT
 */

#ifndef CPU_TRACE_H
#define CPU_TRACE_H

#include "u_perfetto.h"

#include "util/macros.h"

#if defined(HAVE_PERFETTO)

/* note that util_perfetto_is_category_enabled always returns false util
 * util_perfetto_init is called
 */
#define _MESA_TRACE_BEGIN(category, name)                                    \
   do {                                                                      \
      if (unlikely(util_perfetto_is_category_enabled(category)))             \
         util_perfetto_trace_begin(category, name);                          \
   } while (0)

#define _MESA_TRACE_END(category)                                            \
   do {                                                                      \
      if (unlikely(util_perfetto_is_category_enabled(category)))             \
         util_perfetto_trace_end(category);                                  \
   } while (0)

#else

#define _MESA_TRACE_BEGIN(category, name)
#define _MESA_TRACE_END(category)

#endif /* HAVE_PERFETTO */

#if __has_attribute(cleanup) && __has_attribute(unused)

#define _MESA_TRACE_SCOPE_VAR_CONCAT(name, suffix) name##suffix
#define _MESA_TRACE_SCOPE_VAR(suffix)                                        \
   _MESA_TRACE_SCOPE_VAR_CONCAT(_mesa_trace_scope_, suffix)

/* This must expand to a single non-scoped statement for
 *
 *    if (cond)
 *       _MESA_TRACE_SCOPE(...)
 *
 * to work.
 */
#define _MESA_TRACE_SCOPE(name)                                              \
   int _MESA_TRACE_SCOPE_VAR(__LINE__)                                       \
      __attribute__((cleanup(_mesa_trace_scope_end), unused)) =              \
         _mesa_trace_scope_begin(name)

static inline int
_mesa_trace_scope_begin(const char *name)
{
   _MESA_TRACE_BEGIN(name);
   _MESA_GPUVIS_TRACE_BEGIN(name);
   return 0;
}

static inline void
_mesa_trace_scope_end(UNUSED int *scope)
{
   _MESA_GPUVIS_TRACE_END();
   _MESA_TRACE_END();
}

#else

#define _MESA_TRACE_SCOPE(name)

#endif /* __has_attribute(cleanup) && __has_attribute(unused) */

#define MESA_TRACE_SCOPE(name) _MESA_TRACE_SCOPE(name)
#define MESA_TRACE_FUNC() _MESA_TRACE_SCOPE(__func__)

static inline void
util_cpu_trace_init()
{
   util_perfetto_init();
   util_gpuvis_init();
}

#endif /* CPU_TRACE_H */
