#ifndef BOOT_H
#define BOOT_H
#include "hal_api.h"
extern void _start(uintn_t hart_id,tsmp_t* TSMP);

#endif  //BOOT_H