#ifndef STALLION_ASSERT_H
#define STALLION_ASSERT_H

#ifdef NDEBUG
#define assert(ignore) ((void)0)
#else
// TODO: Diagnose failed assertions
#define assert(test) \
  if (!(test)) {\
    abort();\
  }
#endif
#define static_assert _Static_assert
#endif