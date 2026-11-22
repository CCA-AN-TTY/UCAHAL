#ifndef HAL_API_H
#define HAL_API_H

#if defined (__riscv) && (__riscv_xlen == 64)
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef uint64_t uintn_t;
typedef uint64_t reg_t;
typedef unsigned long long size_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long int64_t;

typedef uint64_t uptr_t;
#define assert(exp) ((void)0)
#define tlsf_assert assert
typedef uint64_t uintmax_t;
typedef unsigned long long uintptr_t;
typedef unsigned char bool;
typedef long long ptrdiff_t;
#define CHAR_BIT 8
#define B_BIT ((1ULL<<63))
#define INTERRUPT_BIT ((1ULL << 63))
#define BIT_LEN 64

#define NULL ((void*)0)
#define true ((bool)1)
#define false ((bool)0)

#endif

typedef enum __attribute__ ((packed)){
  memory_reserved = 0,
  memory_hole     = 1,
  memory_avail    = 2,
  memory_mmio     = 3
}memory_type;

typedef struct __attribute__ ((aligned(16))){
  uintn_t base_addr;
  uintn_t len;
  memory_type type;
  uint8_t m_info[15];
}memory_info_t;

typedef struct __attribute__ ((aligned(16))){
  uintn_t base_addr;
  uintn_t addr_len;
  uintn_t avail_mem;
  uintn_t memory_topology_len;
  memory_info_t** memory_map;
  uintn_t core_amounts;
  uintn_t main_core;
  uint8_t aboot_info[8];
}boot_info_t;

typedef struct __attribute__ ((aligned(64))){
  uintn_t  core_id;
  uintn_t  stack_top;
  uintn_t  dcache_block_size;
  uint32_t crc32;
  uint32_t has_cmo;
  void* tlsf_base;
  uint8_t  core_info[24];
}core_info_t;


/* CONSOLE COLOR PARAMETERS */
#define BLACK "\x1b[90m"
#define RED "\x1b[91m"
#define GREEN "\x1b[92m"
#define RESET "\x1b[0m"

#define BG_RED "\x1b[101m"
#define BG_GREEN "\x1b[102m"
#define BG_YELLOW "\x1b[103m"
#define BG_BLUE "\x1b[104m"
#define BG_WHITE "\x1b[107m"


extern  int hal_console_init(void);
extern  __attribute__((no_inline)) void hal_trap_init(unsigned long hartid,uint8_t secondary_hart);

extern void hal_init(uintn_t core_id,void* dtb);
extern void hart_main(uintn_t core_id,void* dtb);
extern __attribute__ ((always_inline)) void hal_store_core_info ( core_info_t* core_info);
extern __attribute__ ((always_inline)) inline core_info_t* get_core_info_ptr();


extern void console_putc(char c);
extern void console_putstr(const char* str);
extern void console_putln(const char* str);
extern void console_puthex(uintn_t hex);
extern void console_set_color();
extern void console_reset_color();
extern void console_putbin(unsigned char val);
extern void console_write(char *args[], int argc);
extern void console_set_color(char* color);


//WIP
extern  void hal_send_ipi(unsigned long core);
//WIP

extern __attribute__ ((always_inline)) uintn_t hal_get_core_id();

extern void hal_shutdown();

// WIP
extern int hal_disk_initialize(void);
extern int hal_disk_status(void);
extern int hal_disk_read(uint8_t* buffer, uintn_t sector_lba, uintn_t count);
extern int hal_disk_write(const uint8_t* buffer, uintn_t sector_lba, uintn_t count);
// WIP

#endif  //HAL_API_H