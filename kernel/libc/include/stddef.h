#ifndef STALLION_LIBC_STDDEF_H
#define STALLION_LIBC_STDDEF_H
#include "stdint.h"

#ifdef STALLION_BITS_32
typedef int32_t ptrdiff_t;
typedef uint32_t size_t;
#else
#ifdef STALLION_BITS_64
typedef int64_t ptrdiff_t;
typedef uint64_t size_t;
#else
#error "Unsupported Stallion architecture."
#endif
#endif

#if !defined(NULL)
#define NULL ((void *)0)
#endif

typedef long double max_align_t;
typedef int wchar_t;

#define offsetof(st, m) \
    ((size_t) ( (char *)&((st *)(0))->m - (char *)0 ))

#endif