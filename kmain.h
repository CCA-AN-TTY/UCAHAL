#ifndef KERNEL_H
#define KERNEL_H
#include "hal_api.h"

extern void kmain(reg_t core_id, boot_info_t* boot_info);
#endif
