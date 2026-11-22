#include "arch/rv64/dtb.h"
#include "memory/mem.h"
#include "arch/rv64/smoldtb.h"

void* copy_get_dtb(uint32_t* dtb){
  if (be32(dtb[0]) != MAGIC) {
    return NULL;
  }
  uint32_t totalsize = be32(dtb[1]);
  void* dt = balloc(totalsize);
  for (uintptr_t i = 0; i<totalsize;i++){
    ((uint8_t*)dt)[i]=((uint8_t*)dtb)[i];
  }
  uint8_t* dtb_addr = (uint8_t*)dtb;
  for(uintn_t i=0;i<totalsize;i++){
    dtb_addr[i] = 0;
  }
  return (void*)dt;
}

int str_sim(const char* one,const char* two){
  uintptr_t i=0;
  while (one[i]!='\0' || two[i]!='\0'){
    if (one[i]!=two[i]){
      return 0;
    }
    i++;
  }
  return 1;
}


