#include "dtb.h"
#include "memory/mem.h"
#include "smoldtb.h"
#include "hal_api.h"

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

void init_parse(tsmp_t dtb){
  void* copy_dtb = copy_get_dtb((uint32_t*)dtb);

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
  // TODO:
  // 1. Parse Soc (plic, uart)
  // 2. Register PLIC/UART addresses.
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

void check_compatibility(){
  console_putln("COMPABILLITY");
  dtb_node* node = dtb_find("cpus");
  dtb_node* child = dtb_get_child(node);
  dtb_node* sibling;
  while(true){
    if (child){
      console_putln(node->name);
      dtb_prop* prop = dtb_find_prop(node,"riscv");
      console_puthex((uintn_t)prop);
      if(prop){
        console_putln(prop->name);
      }
    }else{
      break;
    }
  }
}

void set_memory(boot_info_t* boot_info){
  dtb_node* node = dtb_find("memory");
  uintn_t start_addr,memory_length;
  dtb_prop* property=dtb_find_prop(node,"reg");
  start_addr = ((uintn_t*)property->data)[0];
  memory_length = ((uintn_t*)property->data)[1];
  //check_compatibility();


  
  console_putln("MMAP INIT");
  malign(BIT_LEN >> 2);


  boot_info->memory_topology_len=3;
  malign(BIT_LEN >> 2);

  memory_info_t** memory_info = balloc(sizeof(memory_info_t*)*boot_info->memory_topology_len);
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


  boot_info->memory_map=memory_info;
  boot_info->core_amounts = 1; //for now!
  boot_info->main_core = hal_get_core_id();
  boot_info->avail_mem = memory_info[2]->len;
  boot_info->base_addr=bptr;
  malign(BIT_LEN >> 2);

}


