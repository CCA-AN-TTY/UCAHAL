#include "arch/rv64/drivers/sbi/uart.h"
#include "arch/rv64/drivers/sbi/sbi.h"
#include "../cmd.h"
#include "../kmain.h"
#define CONSOLE_BUFF_LEN 128

char console_buff [CONSOLE_BUFF_LEN];
char text_buff    [CONSOLE_BUFF_LEN*3];

inline int hal_console_init(){
  *UART_REG_RXCTRL = 0x00000001; 
  *UART_REG_IE     = 0x00000002;
  return 0;
}

sbi_ret_t console_getc(){
  sbi_args_t sbi_args = {0};
  sbi_args.EID = DBCN;
  sbi_args.FID = 1;
  sbi_args.a0 = CONSOLE_BUFF_LEN;
  sbi_args.a1 = (unsigned long)&console_buff;
  sbi_args.a2 = 0;
  sbi_ret_t ret = sbi_call(&sbi_args);
  return ret;
};

void console_putc(char c){
  sbi_args_t sbi_args = {0};
  sbi_args.EID = DBCN;
  sbi_args.FID = 2; //console wryte byte
  sbi_args.a0 = (unsigned char)c;
  
  sbi_call(&sbi_args);
}

void console_putstr(const char *str) {
  sbi_args_t sbi_args ={0};
  unsigned long len = 0;
  while (str[len] != '\0') len++;
  sbi_args.EID = DBCN;
  sbi_args.FID = 0; //Console putchar
  sbi_args.a0 = len;
  sbi_args.a1 = (unsigned long)str;
  sbi_call(&sbi_args);
}

void console_putln(const char *str) {
  if (!str) {
    return;
  }
  console_putstr(str);
  console_putc('\n');
}

void console_isr(){
  console_putc((char)0x56);
  sbi_ret_t ret = console_getc();
  if(ret.error){
    console_putln("ERROR");
    return;
  }
  uintn_t i=curr_char;
  uintn_t t=0;
  while(i<ret.value){
    if (console_buff[i] == 0x7F || console_buff[i] == '\b') {
      text_buff[t]='\b';
      t+=1;
      text_buff[t]=' ';
      t+=1;
      text_buff[t]='\b';
    }else if (console_buff[i] == '\n' || console_buff[i] == '\r') {

      text_buff[t]='\r';
      t+=1;
      text_buff[t]='\n';
      command(&console_buff,curr_char);
      clear_buffer();
    }else {
      text_buff[t]=console_buff[i];
    }
    console_buff[i]='\0';
    //make it to the kmain buffer
    i++;
    t+=1;
  }
  curr_char++;

  console_putstr(text_buff);


  i=0;
  while(i<(t+1)){
    text_buff[i]='\0';
    i++;
  }
}


void console_puthex(uintn_t val) { //for risc v 64
  const char *h = "0123456789ABCDEF";
  char s[17];
  for (int i = 0; i < 16; i++) {
    s[15 - i] = h[val & 0xF];
    val >>= 4;
  }
  s[16] = '\0';
  console_putstr("0x");
  console_putstr(s);
  console_putc('\n');
}

void console_putbin(unsigned char val) {
  for (unsigned char i = 0; i < 8; i++) {
    console_putc('0' + ((val & (1 << i)) >> i));
  }
  console_putc('\n');
}

void console_write(char *args[], int argc) {
  console_putc('\n');

  if (argc < 2) {
    console_putln("ERROR: arguments are not enough than needed");
    return;
  }
  for (int i = 1; i < argc; i++) {
    console_putstr(args[i]);
    console_putc(' ');
  }
  console_putc('\n');
}
void console_clear_screen() {
  console_putstr("\x1B[2J\x1B[H");

}


void console_set_color(char *color) { console_putstr(color); }

void console_reset_color() { console_putstr(RESET); }
