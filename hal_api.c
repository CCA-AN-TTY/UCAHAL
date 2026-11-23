#include "hal_api.h"
#include "arch/rv64/drivers/sbi/sbi.h"
#include "memory/mem.h"
#include "arch/rv64/int/s_trap.h"
#include "arch/rv64/hart.h"
#include "../boot.h"
#include "../kmain.h"


extern char _end[];


void hart_main(uintn_t core_id,void* TSMP){
  if(!core_id){
    console_putln("\"ZERO CORE\" ERROR");
    while(1){
      asm volatile ("wfi");
    }
  }
  console_putln("hart main core id");
  console_puthex(core_id);
  reg_t sp;
  asm volatile ("mv sp,%0" : "=r"(sp));
  console_puthex(sp);
  core_info_t core_info = {0};
  core_info.core_id=core_id;
  asm volatile ("mv tp,%0" :: "r"(&core_id) : "tp");
  hal_trap_init(core_id,1);
  kmain(core_id,TSMP); //WIP
  while(1){
    asm volatile ("wfi");
  }
}



void hal_init(uintn_t core_id,tsmp_t TSMP){
  asm volatile ("FENCE.I" ::: "memory");
  core_info_t core_info = {0};
  core_info.core_id=core_id;
  core_info.dcache_block_size=64;
  
  asm volatile ("mv tp,%0" :: "r"(&core_info) : "tp","memory");
  console_puthex(core_id);
  binit((uintn_t)_end);
  init_parse(TSMP);
  console_putln("CORE ID:");
  hal_trap_init(core_id,0);

  asm volatile ("FENCE.I" ::: "memory");

  boot_info_t boot_info = {0};

  set_memory(&boot_info);

  
  kmain(core_id,&boot_info);

  
  

  
  while(1){
    asm volatile ("wfi");
  }
}

uintn_t hal_get_core_id(){
  core_info_t* core_info;
  asm volatile ("mv %0,tp" : "=r"(core_info));
  return core_info->core_id;
}

__attribute__ ((always_inline)) inline core_info_t* get_core_info_ptr(){
  core_info_t* core_info;
  asm volatile ("mv %0,tp" : "=r"(core_info) :: "memory");
  return core_info;
}




