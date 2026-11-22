#include "hal_api.h"
#include "arch/rv64/drivers/sbi/sbi.h"
#include "memory/mem.h"
#include "arch/rv64/int/s_trap.h"
#include "arch/rv64/hart.h"
#include "../boot.h"
#include "../kmain.h"
#include "arch/rv64/smoldtb.h"

extern char _end[];


void hart_main(uintn_t core_id,void* dtb){
  if(!core_id){
    console_putln("\"ZERO CORE\" ERROR");
    while(1){
      asm volatile ("wfi");
    }
  }
  console_putln("hart main core id");
  console_puthex(core_id);
  console_putln("hello");
  reg_t sp;
  asm volatile ("mv sp,%0" : "=r"(sp));
  console_puthex(sp);
  core_info_t core_info = {0};
  core_info.core_id=core_id;
  asm volatile ("mv tp,%0" :: "r"(&core_id) : "tp");
  hal_trap_init(core_id,1);
  kmain(core_id,dtb);
  while(1){
    asm volatile ("wfi");
  }
}

static void* copy_get_dtb(uint32_t* dtb){
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

static int str_sim(const char* one,const char* two){
  uintptr_t i=0;
  while (one[i]!='\0' || two[i]!='\0'){
    if (one[i]!=two[i]){
      return 0;
    }
    i++;
  }
  return 1;
}

void hal_init(uintn_t core_id,void* dtb){
  asm volatile ("FENCE.I" ::: "memory");
  core_info_t core_info = {0};
  core_info.core_id=core_id;
  core_info.dcache_block_size=64;
  
  asm volatile ("mv tp,%0" :: "r"(&core_info) : "tp","memory");
  console_puthex(core_id);
  binit((uintn_t)_end);
  void* copy_dtb = copy_get_dtb((uint32_t*)dtb);
  console_putln("START");
  hal_trap_init(core_id,0);


  dtb_ops ops = {
    .malloc = balloc,
    .free = bfree,
    .on_error = console_putln
  };

  if (!dtb_init((uintn_t)copy_dtb, ops)) {
    console_putln("ERROR COULDN\'T INIT DTB PARSING");
    while(1){
      asm volatile ("wfi");
    }
  }
  asm volatile ("FENCE.I" ::: "memory");

  dtb_node* node = dtb_find("memory");
  dtb_prop* property=node->props;
  uintn_t start_addr,memory_length;

  console_putln("NAME");
  while(true){
    console_putln(property->name);
    if(str_sim(property->name,"reg")){
      console_putln("FOUND");
      //console_puthex(property->length>>((BIT_LEN>>5)|1));
      console_putln("DATA");
      start_addr = ((uintn_t*)property->data)[0];
      memory_length = ((uintn_t*)property->data)[1];
      break;
    }else{
      console_putln("next");
      console_puthex(property->length);
      if(property->next){
        property=property->next;
      }else{
        break;
      }
      console_puthex((uintn_t)property->next);
    }
  }

  console_putln("MMAP INIT");
  malign(16);
  void* tlsf_base = balloc(sizeof(void*));
  core_info.tlsf_base=tlsf_base;

  boot_info_t boot_info = {0};
  boot_info.memory_topology_len=3;
  malign(16);

  memory_info_t** memory_info = balloc(sizeof(memory_info_t*)*boot_info.memory_topology_len);
  memory_info_t* memory_info_mmio = balloc(sizeof(memory_info_t));
  memory_info_mmio->base_addr = 0x0;
  memory_info_mmio->len = start_addr;
  memory_info_mmio->type=memory_mmio;
  memory_info[0]=memory_info_mmio;

  memory_info_t* memory_info_begin = balloc(sizeof(memory_info_t));

  memory_info_begin->base_addr = start_addr;
  memory_info_begin->len = (bptr+sizeof(memory_info_t)) - start_addr;
  memory_info_begin->type=memory_reserved;
  memory_info[1]=memory_info_begin;

  memory_info_t* memory_info_avail = balloc(sizeof(memory_info_t));
  memory_info_avail->base_addr = bptr;
  memory_info_avail->len = memory_length-memory_info[1]->len;
  memory_info_avail->type=memory_avail;
  memory_info[2]=memory_info_avail;


  boot_info.memory_map=memory_info;
  boot_info.core_amounts = 1; //for now!
  boot_info.main_core = core_id;
  boot_info.avail_mem = memory_info[2]->len;
  boot_info.base_addr=bptr;


  
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




