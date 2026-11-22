#ifndef TRAP_H
#define TRAP_H
#include "hal_api.h"

#define INTERVAL 1000000 //1 sec
#define SIE ((1<<1))

#define SIE_SSIE ((1<<1))
#define SIE_STIE ((1<<5))
#define SIE_SEIE ((1<<9))

//extern  __attribute__((no_nline)) void hal_trap_init(unsigned long hartid,uint8_t secondary_hart);
extern reg_t trap_hand(reg_t sepc, reg_t scause, reg_t stval) ;
extern __attribute__((noinline)) void
kernel_fault(char *text, uptr_t ERROR_CODE, unsigned char fof);
#endif
