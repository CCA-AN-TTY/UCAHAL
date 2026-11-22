#include "arch/rv64/drivers/sbi/time.h"
#include "arch/rv64/int/trap.h"
#include "arch/rv64/drivers/sbi/sbi.h"
#include "hal_api.h"
unsigned long time_int = 0;

void time_call(unsigned long interval){
  reg_t curr_time;
  asm volatile ("csrr %0,time" : "=r"(curr_time));
  sbi_args_t sbi_args = {0};
  sbi_args.EID = SBI_TIME;
  sbi_args.FID = 0;
  sbi_args.a0 = curr_time + interval;
  sbi_call(&sbi_args);
}

