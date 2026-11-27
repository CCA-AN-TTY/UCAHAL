#include "drivers/sbi/uart.h"
#include "drivers/sbi/sbi.h"
#include "../cmd.h"
#include "../kmain.h"
#define CONSOLE_BUFF_LEN 128

char console_buff [CONSOLE_BUFF_LEN];
char text_buff    [CONSOLE_BUFF_LEN<<1];
char command_buff [CONSOLE_BUFF_LEN<<1];

inline int hal_console_init(){
  *UART_REG_RXCTRL = 1; 
  *UART_REG_IE     = 2;
  return 0;
}

inline sbi_ret_t console_getc(){
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
  sbi_args.FID = 0;
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
uint32_t char_pos=0;
uint32_t t_pos=0;
__attribute__((retain)) inline void execute_command(char* buff,uint16_t char_p){
  if (  text_buff[t_pos]=='\n' || text_buff[t_pos] == '\r'){
    text_buff[t_pos]='\0';
  }
  console_putstr(text_buff);
  command(buff,char_p);
}

void console_isr(){
  sbi_ret_t ret = console_getc();
  if(ret.error){
    console_putln("ERROR IN EXECUTING UART ISR!");
    return;
  }
  uintn_t i=0;
  uintn_t t=0;
  uintn_t c=char_pos;
  while(i<ret.value){
    if (console_buff[i%CONSOLE_BUFF_LEN] == 0x7F || console_buff[i%CONSOLE_BUFF_LEN] == '\b') {
      text_buff[t]='\b';
      t+=1;
      text_buff[t]=' ';
      t+=1;
      text_buff[t]='\b';
      command_buff[c]='\0';
      if(c>0){
        c-=1&c;
      }
      t+=1;
    }else if (console_buff[i%CONSOLE_BUFF_LEN] == '\n' || console_buff[i%CONSOLE_BUFF_LEN] == '\r') {

      text_buff[t]='\r';
      t+=1;
      text_buff[t]='\n';
      char_pos=c;
      t_pos=t;
      
      execute_command(&command_buff,char_pos);
      console_putc('\n');
      uint16_t base=0;
      while(base<(t+1)){
        text_buff[base]='\0';
        base++;
      }
      base=0;
      while(base<(c+1)){
        command_buff[base]='\0';
        base++;
      }
      t=0;
      t_pos=0;
      c=0;
    }else {
      text_buff[t]=console_buff[i];
      command_buff[c]=console_buff[i];
      t+=1;
      c+=1;
    }
    console_buff[i]='\0';
    i++;
  }

  char_pos=c;


  if(t){
    console_putstr(text_buff);
  }
  i=0;
  while(i<(t+1)){
    console_buff[i]='\0';
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
