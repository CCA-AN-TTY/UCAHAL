#ifndef HAL_PARSE_H
#define HAL_PARSE_H

#if defined (__riscv) && (__riscv_xlen == 64)
  #if defined (DTB)
    #pragma message("DTB RV64 ON")
    typedef void* tsmp_t;
    #include "arch/risc_v/rv64/dtb.h"
  #else 
    #pragma message("DTB RV64 OFF")
  #endif

#else if defined (__riscv) && (__riscv_xlen == 32)
  #if defined (DTB)
    #pragma message("DTB RV32 ON")
    typedef void* tsmp_t;
    //#include "arch/risc_v/rv32/dtb.h"
  #else
    pragma message("DTB RV32 OFF")
  #endif
#endif 

#endif  //HAL_PARSE_H