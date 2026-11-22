#include "arch/rv64/hart.h"
#include "arch/rv64/drivers/sbi/sbi.h"
#include "hal_api.h"

sbi_ret_t hart_start(uintn_t hartid,uintn_t addr,uintn_t opaque){
  sbi_args_t sbi_args = {0};
  sbi_args.EID = HSM;
  sbi_args.FID = 0;
  sbi_args.a0 = hartid;
  sbi_args.a1 = addr;
  sbi_args.a2 = opaque;
  sbi_ret_t sbi_ret = sbi_call(&sbi_args);
  return sbi_ret;
}
