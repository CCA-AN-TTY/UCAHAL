#include "hal_api.h"

void kmain(reg_t core_id, boot_info_t* boot_info){
    console_putln("HELLO WORLD FROM KERNEL");
    while(1){
        asm volatile ("wfi");
    }
}

void command(char *str,unsigned short curr_char){
    //dummy function
}
