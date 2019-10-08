#ifndef STALLION_LIBC_STDINT_H
#define STALLION_LIBC_STDINT_H

#ifdef STALLION_BITS_32
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
#else
#ifdef STALLION_BITS_64
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
#else
#error "Unsupported Stallion architecture."
#endif

#endif

#endif