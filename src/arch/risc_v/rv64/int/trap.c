#include "int/trap.h"
#include "drivers/sbi/uart.h"
#include "control/power.h"
#include "drivers/sbi/time.h"
#include "int/plic.h"
#include "int/s_trap.h"
#include "hal_api.h"



__attribute__((always_inline)) inline static void set_protocol_0_color() {
  console_set_color(BG_WHITE);
  console_set_color(BLACK);
}
__attribute__((always_inline)) inline static void set_protocol_1_color() {
  console_set_color(BG_BLUE);
}

__attribute__((always_inline)) inline static void set_protocol_2_color() {
  console_set_color(BG_YELLOW);
}
__attribute__((awlays_inline)) inline static void set_protocol_3_color() {
  console_set_color(BG_RED);
}
void hal_trap_init(unsigned long hartid,uint8_t secondary_hart) {
  volatile reg_t id = hartid;
  time_call(INTERVAL);
  asm volatile("csrw stvec,%0" ::"r"(((uintn_t)s_trap)));
  volatile reg_t stuff;
  asm volatile("csrr %0,sstatus" : "=r"(stuff) :: "memory");
  asm volatile("csrw sstatus,%0" ::"r"((stuff | SIE)) : "memory");
  asm volatile("csrw sip,zero");
  stuff = 0;
  asm volatile("csrr %0,sie" : "=r"(stuff) :: "memory");
  if (!secondary_hart){
    stuff |= SIE_SSIE | SIE_SEIE | SIE_STIE; 
  }else{
    stuff |= SIE_SSIE;
  }
  asm volatile("csrw sie,%0" ::"r"(stuff) : "memory");
  init_plic(id);
  hal_console_init();
}


static __attribute__((retain)) inline void handle_illegal_instruction(void) {
  register uintptr_t a0 __asm__("a0") = 0x2026;
  console_putln("ILLEGAL INSTRUCTION! ERROR");
  __asm__ volatile("ebreak" : : "r"(a0) : "memory");
}
reg_t trap_hand(reg_t sepc, reg_t scause, reg_t stval) {
  reg_t epc = sepc;
  core_info_t* core_info_ptr={0};
  asm volatile("mv %0,tp" : "=r"(core_info_ptr));
  reg_t id = core_info_ptr->core_id;
  //console_putln("trap");
  //console_puthex(id);
  if (scause & INTERRUPT_BIT) {
    reg_t code = scause & ~B_BIT;

    switch (code) {
    case 1:
      console_putln("[TRAP] SSOFT INTERUPT");
      break;

    case 5:
      //console_putln("[TRAP] STimer interrupt!");
      time_call(INTERVAL);

      break;

    case 9:
      //console_putln("[TRAP] SExternal interrupt!");
      isr(id);
      break;

    default:
      console_putln("[TRAP] Unknown interrupt");
      break;
    }
    return epc;
  } else {
    reg_t code = scause & 0xff;
    set_protocol_3_color();
    uint8_t fof=0;
    console_putln("[EXCEPTION] Exception occurred!");
    switch (code) {
    case 0:
      console_putln("[TRAP] Instruction address misaligned");
      fof=1;
      break;
    case 1:
      console_putln("[TRAP]  Instruction access fault");
      fof=1;
      break;
    case 2:
      handle_illegal_instruction();
//      console_putln("[TRAP] ILLEGAL INTSTRUCTION!");
      fof=1;
      break;
    case 3:
      set_protocol_0_color();
      console_putln("[TRAP] BREAKPOINT");
      break;
    case 4:
      console_putln("[TRAP] LOAD ADRESS MISSALIGNED!");
      fof=1;
      break;
    case 5:
      console_putln("[TRAP] LOAD ADRESS FAULT");
      fof=1;
      break;
    case 6:
      console_putln("[TRAP] Store/AMO adress missalign");
      fof=1;
      break;
    case 7:
      console_putln("[TRAP] Store/AMO access fault");
      fof=1;
      break;
    case 8:
      console_putln("[TRAP] Enviorment call from U-MODE");
      break;
    case 9:
      console_putln("[TRAP] Enviorment call from S-MODE");
      break;
    case 11:
      console_putln("[TRAP] Enviorment call from M-MODE");
      break;
    case 12:
      console_putln("[TRAP] Instruction page fault");
      fof=1;
      break;
    case 13:
      console_putln("[TRAP] Load page fault");
      fof=1;
      break;
    case 15:
      console_putln("[TRAP] Store/AMO page fault");
      fof=1;
      break;
    default:
      set_protocol_1_color();
      console_putln("[TRAP] Hardware specified exception");
      break;
    }
    console_putstr("sepc: ");
    console_puthex(sepc);
    console_putstr("STVAL: ");
    console_puthex(stval);
    console_putstr("SCAUSE: ");
    console_puthex(scause);
    console_putstr("EPC: ");
    console_puthex(epc);
    console_putln("System halted.");
    kernel_fault("System, execute code error",0x100505,1);
    while(1){
        asm volatile("wfi");
    }
  }
}

void kernel_fault(char *text, uptr_t ERROR_CODE, unsigned char fof) {
  set_protocol_3_color();
  console_putln("KERNEL FAULT");
  console_putstr("Reason: ");
  console_putln(text);
  console_putln("ERROR_CODE");
  console_puthex(ERROR_CODE);
  console_putln("CORE_ID");
  console_puthex(hal_get_core_id());

  if (fof) {
    console_putln("SYSTEM SHUTDOWN");
    hal_shutdown();
    console_putln("ERROR IN SYSTEM SHUTDOWN");

  }
  asm volatile("ebreak" ::: "memory");
}