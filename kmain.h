#ifndef KERNEL_H
#define KERNEL_H
#include "hal_api.h"
extern unsigned short curr_char;
extern char buff[0x1000];

extern void clear_buffer();
extern void kmain(reg_t core_id, boot_info_t* boot_info);
#endif
