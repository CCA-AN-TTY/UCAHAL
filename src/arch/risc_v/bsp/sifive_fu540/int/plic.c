#include "int/plic.h"
#include "drivers/sbi/uart.h"

__attribute__((always_inline)) inline static void set_priority(unsigned int irq, unsigned char priority) {
  *(volatile unsigned int *)(PLIC_PRIORITY + (irq << 2)) = priority;
}

__attribute__((always_inline)) inline static void enable_irq(unsigned long hart,unsigned int irq) {
  volatile unsigned int *reg = (unsigned int *)PLIC_ENABLE(hart);
  *reg = *reg | (1 << irq);
}

__attribute__((always_inline)) inline static void
thhold_en(unsigned long hart) {
  *(volatile unsigned int *)PLIC_THRESHOLD(hart) = 0;
}

__attribute__((always_inline)) inline static unsigned int
claim(unsigned long hart) {
  return *(volatile unsigned int *)PLIC_CLAIM(hart);
}

__attribute__((always_inline)) inline static void complete(unsigned long hart,
                                                           unsigned int irq) {
  *(volatile unsigned int *)PLIC_CLAIM(hart) = irq;
}

void init_plic(unsigned long hart) {
  set_priority(UART_IRQ, 1);
  enable_irq(hart, UART_IRQ);
  thhold_en(hart);
}

void isr(unsigned long hart) {
  unsigned int irq = claim(hart);
  switch (irq) {
  case UART_IRQ:
    console_isr();
    break;
  default:
    break;
  }

  complete(hart, irq);
}
