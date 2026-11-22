#include "hal_api.h"
#include "arch/rv64/drivers/sbi/sbi.h"
void hal_shutdown() { 
  sbi_args_t sbi_args;
  sbi_args.EID = SBI_SHUTDOWN_EID;
  sbi_call(&sbi_args);
}
