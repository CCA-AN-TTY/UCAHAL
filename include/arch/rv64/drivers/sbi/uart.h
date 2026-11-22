#ifndef UART_H
#define UART_H

#include "hal_api.h"
#include "arch/rv64/drivers/sbi/sbi.h"

#define UART0_BASE 0x10010000UL
#define UART_REG_RXCTRL ((volatile unsigned int *)(UART0_BASE + 0x0C))
#define UART_REG_IE     ((volatile unsigned int *)(UART0_BASE + 0x10))
#define UART_REG_IP     ((volatile unsigned int *)(UART0_BASE + 0x14))


extern __attribute__((noinline)) inline int hal_console_init();
extern __attribute__((noinline)) void console_isr();
extern __attribute__((always_inline)) inline sbi_ret_t console_getc();
extern __attribute__((always_inline)) inline void console_putc(char c);
extern __attribute__((noinline)) void console_putstr(const char *str);
extern __attribute__((noinline)) void console_putln(const char *str);
extern __attribute__((noinline)) void console_puthex(uintn_t hex);
extern __attribute__((noinline)) void console_putbin(unsigned char val);
extern __attribute__((noinline)) void console_write(char *args[], int argc);
extern __attribute__((always_inline)) void console_set_color(char *color);
extern __attribute__((always_inline)) void console_reset_color();

#endif //UART_H