#ifndef HAL_PARSE_H
#define HAL_PARSE_H

#if defined (__riscv) && (__riscv_xlen == 64)
#if defined (DTB)
  #pragma message("DTB ON")
  typedef void* tsmp_t;
  #include "arch/rv64/dtb.h"
#else
  #pragma message("DTB OFF")
#endif

#endif 

#endif  //HAL_PARSE_H