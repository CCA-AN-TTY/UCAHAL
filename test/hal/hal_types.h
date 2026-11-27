#ifndef HAL_TYPES_H
#define HAL_TYPES_H

#if defined (__riscv) && (__riscv_xlen == 64)
  typedef unsigned char uint8_t;
  typedef unsigned short uint16_t;
  typedef unsigned int uint32_t;
  typedef unsigned long uint64_t;
  typedef uint64_t uintn_t;
  typedef uint64_t reg_t;
  typedef unsigned long long size_t;
  typedef signed char int8_t;
  typedef signed short int16_t;
  typedef signed int int32_t;
  typedef signed long int64_t;

  typedef uint64_t uptr_t;
  #define assert(exp) ((void)0)
  #define tlsf_assert assert
  typedef uint64_t uintmax_t;
  typedef unsigned long long uintptr_t;
  typedef unsigned char bool;
  typedef long long ptrdiff_t;
  #define CHAR_BIT 8
  #define B_BIT ((1ULL<<63))
  #define INTERRUPT_BIT ((1ULL << 63))
  #define BIT_LEN 64

  #define NULL ((void*)0)
  #define true ((bool)1)
  #define false ((bool)0)

  #define ANSI
#else if defined (__riscv) && (__riscv_xlen == 32)
  typedef unsigned char uint8_t;
  typedef unsigned short uint16_t;
  typedef unsigned int uint32_t;
  typedef uint32_t uintn_t;
  typedef uint32_t reg_t;
  typedef signed char int8_t;
  typedef signed short int16_t;
  typedef signed int int32_t;

  typedef uint32_t uptr_t;
  #define assert(exp) ((void)0)
  #define tlsf_assert assert
  typedef uint32_t uintmax_t;
  typedef unsigned int uintptr_t;
  typedef unsigned char bool;
  typedef int ptrdiff_t;
  #define CHAR_BIT 4
  #define B_BIT ((1ULL<<31))
  #define INTERRUPT_BIT ((1ULL << 31))
  #define BIT_LEN 32

  #define NULL ((void*)0)
  #define true ((bool)1)
  #define false ((bool)0)

  #define ANSI

#endif

#if defined (ANSI)
  #define BLACK "\x1b[90m"
  #define RED "\x1b[91m"
  #define GREEN "\x1b[92m"
  #define YELLOW "\x1b[93m"
  #define BLUE "\x1b[94m"
  #define MAGENTA "\x1b[95m"
  #define CYAN "\x1b[96m"
  #define WHITE "\x1b[97m"


  #define RESET "\x1b[0m"

  #define BG_RED "\x1b[101m"
  #define BG_GREEN "\x1b[102m"
  #define BG_YELLOW "\x1b[103m"
  #define BG_BLUE "\x1b[104m"
  #define BG_MAGENTA "\x1b[105m"
  #define BG_CYAN "\x1b[106m"
  #define BG_WHITE "\x1b[107m"

#endif




#endif  //HAL_TYPES_H