#ifndef HAL_API_H
#define HAL_API_H

#include "hal_types.h"

typedef enum __attribute__ ((packed)){
  memory_reserved = 0,
  memory_hole     = 1,
  memory_avail    = 2,
  memory_mmio     = 3
}memory_type;

typedef struct __attribute__ ((aligned(BIT_LEN >> 2))){
  uintn_t base_addr;
  uintn_t len;
  memory_type type;
  uint8_t m_info[(BIT_LEN>>2)-1];
}memory_info_t;

typedef struct __attribute__ ((aligned(BIT_LEN >> 2))){
  uintn_t base_addr;
  uintn_t addr_len;
  uintn_t avail_mem;
  uintn_t memory_topology_len;
  memory_info_t** memory_map;
  uintn_t core_amounts;
  uintn_t main_core;
  uint8_t aboot_info[sizeof(uintn_t)];  //For now
}boot_info_t;

#include "hal_spec.h"

typedef struct __attribute__ ((aligned(64))) core_info_t{
  uintn_t  core_id;
  uintn_t  stack_top;
  uintn_t  dcache_block_size;
  uintn_t  cpu_id;
  void* tlsf_base;
  main_registers_t* main_registers;
  uint8_t  core_info[(64-(((BIT_LEN<<3)*6)&63))]; 
}core_info_t;



#include "hal_parse.h"


extern uint8_t flags;

#define CLZ_SUPPORT (1<<0)



extern  int hal_console_init(void);
extern  __attribute__((no_inline)) void hal_trap_init(unsigned long hartid,uint8_t secondary_hart);

// Topological-Semantic Mechanism Pointer (TSMP)

extern void hal_init(uintn_t core_id,void* TSMP);

extern uint8_t highest_bit_index_64(uint64_t x);

//got from wikepedia, source: https://en.wikipedia.org/wiki/De_Bruijn_sequence
//under CC-BY-SA 4.0 START LINE
extern uint8_t highest_bit_index_32(uint32_t v);
//got from wikepedia, source: https://en.wikipedia.org/wiki/De_Bruijn_sequence
//under CC-BY-SA 4.0 END LINE



extern void console_putc(char c);
extern void console_putstr(const char* str);
extern void console_putln(const char* str);
extern void console_puthex(uintn_t hex);
extern void console_reset_color();
extern void console_putbin(unsigned char val);
extern void console_write(char *args[], int argc);
extern void console_set_color(char* color);


//WIP
extern  void hal_send_ipi(unsigned long core);
//WIP


extern void hal_shutdown();

// WIP
extern int hal_disk_initialize(void);
extern int hal_disk_status(void);
extern int hal_disk_read(uint8_t* buffer, uintn_t sector_lba, uintn_t count);
extern int hal_disk_write(const uint8_t* buffer, uintn_t sector_lba, uintn_t count);
// WIP

#endif  //HAL_API_H