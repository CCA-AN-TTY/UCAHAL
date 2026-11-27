#include "drivers/sbi/time.h"
#include "int/trap.h"
#include "drivers/sbi/sbi.h"
#include "hal_api.h"
uintn_t time_int = 0;

void time_call(unsigned long interval){
  reg_t curr_time;
  asm volatile ("csrr %0,time" : "=r"(curr_time));
  sbi_args_t sbi_args = {0};
  sbi_args.EID = SBI_TIME;
  sbi_args.FID = 0;
  sbi_args.a0 = curr_time + interval;
  sbi_call(&sbi_args);
}

