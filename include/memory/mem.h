#ifndef MEM_H
#define MEM_H

#include "hal_api.h"
extern uintn_t bptr;
extern void binit(uintn_t addr);
extern void* balloc(size_t size);
extern void bfree(void* ptr, size_t size);
extern unsigned long malign(uintn_t size);

#endif  //MEM_H