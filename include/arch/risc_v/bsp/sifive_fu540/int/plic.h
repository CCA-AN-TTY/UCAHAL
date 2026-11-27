#ifndef PLIC_H
#define PLIC_H


#define PLIC_ADDR 0xc000000UL
#define PLIC_PRIORITY PLIC_ADDR
#define PLIC_PENDING (PLIC_ADDR + 0x1000UL)
#define PLIC_ENABLE(hart) (PLIC_ADDR + 0x2000UL + ((hart) * 0x100UL))
#define PLIC_CONTEXT(hart) (PLIC_ADDR + 0x200000UL + (hart) * 0x2000UL)
#define PLIC_THRESHOLD(hart) (PLIC_CONTEXT(hart) + 0x0UL)
#define PLIC_CLAIM(hart) (PLIC_CONTEXT(hart) + 0x4UL)

#define UART_IRQ 4
extern __attribute__((always_inline)) inline void init_plic(unsigned long hart);
extern __attribute__((noinline)) void isr(unsigned long hart);

#endif
