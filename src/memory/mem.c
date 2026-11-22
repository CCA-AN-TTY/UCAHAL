#include "memory/mem.h"
#include "hal_api.h"

uintn_t bptr;

void binit(uintn_t addr){
  bptr = addr;
}
void* balloc(size_t size){
  void* ptr = (void*)bptr;
  bptr+=size;
  return ptr;
}
void bfree(void* ptr, size_t size){
  //dummy function
}
unsigned long malign(uintn_t size){
  uintn_t res = bptr;
  res+=(size-1);
  res&=~(size-1);
  bptr=res;
  return res;
}
