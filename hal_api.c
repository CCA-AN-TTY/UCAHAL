#include "hal_api.h"
#include "memory/mem.h"
#include "hart.h"
#include "../boot.h"
#include "../kmain.h"


extern char _end[];


//got from wikepedia, source: https://en.wikipedia.org/wiki/De_Bruijn_sequence
//under CC-BY-SA 4.0 START LINE
static const uint8_t clz32_table[32] = { 
         0,  1, 16,  2, 29, 17,  3, 22, 30, 20, 18, 11, 13,  4,  7, 23,
        31, 15, 28, 21, 19, 10, 12,  6, 14, 27,  9,  5, 26,  8, 25, 24,
};

static uint32_t highest_bit_32(uint32_t n) 
{
    n |= (n >>  1);
    n |= (n >>  2);
    n |= (n >>  4);
    n |= (n >>  8);
    n |= (n >> 16);
    return n - (n >> 1);
}

uint8_t highest_bit_index_32(uint32_t v)
{

  uint8_t position= clz32_table[(highest_bit_32(v) * 0x06EB14F9U) >> 27];
  return position;
}

//got from wikepedia, source: https://en.wikipedia.org/wiki/De_Bruijn_sequence
//under CC-BY-SA 4.0 END LINE

static const int clz64_table[64] = {
    63, 62, 61, 61, 60, 59, 59, 57,
    59, 57, 56, 55, 54, 53, 52, 50,
    53, 51, 49, 48, 47, 46, 45, 43,
    44, 42, 41, 40, 39, 38, 37, 35,
    36, 34, 33, 32, 31, 30, 29, 27,
    28, 26, 25, 24, 23, 22, 21, 19,
    20, 18, 17, 16, 15, 14, 13, 11,
    12, 10,  9,  8,  7,  6,  5,  3,
};

uint8_t highest_bit_index_64(uint64_t x) {
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;  
    return clz64_table[(x * 0x03F79D71B4CB0A89ULL) >> 58];
}   

uint8_t flags=0;

uint8_t (*highest_bit_index)(uintn_t);
//TODO: in dtb.c in "check_compatibility" write a programm to find all extentions avaliable in each core



inline uint8_t branchless_equal_64(uint64_t num1,uint64_t num2){
  uint8_t result = num1 ^ num2;
  result = (result<<1) >> highest_bit_index_64(result);
  return (((~result)<<PRIOR_BIT_LEN)>>PRIOR_BIT_LEN); //WIP
}


void hal_init(uintn_t core_id,tsmp_t TSMP){
  asm volatile ("FENCE.I" ::: "memory");
  core_info_t core_info = {0};
  core_info.core_id=core_id;
  core_info.dcache_block_size=64;
  set_core_info_ptr(&core_info);
  console_puthex(core_id);
  binit((uintn_t)_end);
  init_parse(TSMP);
  console_putln("CORE ID:");
  hal_trap_init(core_id,0);
  init_spec();
  console_putc('\a');


  asm volatile ("FENCE.I" ::: "memory");

  boot_info_t boot_info = {0};

  set_memory(&boot_info);

  
  kmain(core_id,&boot_info);

  
  

  
  while(1){
    asm volatile ("wfi");
  }
}





