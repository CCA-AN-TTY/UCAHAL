#ifndef HAL_PARSE_H
#define HAL_PARSE_H
#include "hal_api.h"
#if defined(UEFI)
  typedef memory_info_t* tsmp_t;
  #include "uefi_parse.h" //for the future
#else
  #if defined (__riscv) && (__riscv_xlen == 64)
    #if defined (DTB)
      #pragma message("DTB RV64 ON")
      typedef void* tsmp_t;
      #include "dtb.h"
    #else 
      #pragma message("DTB RV64 OFF")
    #endif

  #elif defined (__riscv) && (__riscv_xlen == 32)
    #if defined (DTB)
      #pragma message("DTB RV32 ON")
      typedef void* tsmp_t;
      //#include "dtb.h"
    #else
      pragma message("DTB RV32 OFF")
    #endif
  #endif 
#endif

#endif  //HAL_PARSE_H