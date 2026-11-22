#ifndef HART_H
#define HART_H

#include "arch/rv64/drivers/sbi/sbi.h"
#include "hal_api.h"

extern sbi_ret_t hart_start(uintn_t hartid,uintn_t addr,uintn_t opaque);


#endif  //HART_H