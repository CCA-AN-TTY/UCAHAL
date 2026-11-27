#include "spec.h"
#include "memory/mem.h"

__attribute__ ((always_inline)) inline uintn_t hal_get_core_id(){
  core_info_t* core_info;
  asm volatile ("mv %0,tp" : "=r"(core_info));
  return core_info->core_id;
}

__attribute__ ((always_inline)) inline core_info_t* get_core_info_ptr(){
  core_info_t* core_info;
  asm volatile ("mv %0,tp" : "=r"(core_info) :: "memory");
  return core_info;
}
__attribute__ ((always_inline)) inline void set_core_info_ptr(core_info_t* core_info){
  asm volatile ("mv tp,%0" :: "r"(&core_info) : "tp","memory");
}




__attribute__ ((always_inline)) inline void reg_snapshot(){
  core_info_t* core_info = get_core_info_ptr();
  asm volatile ("mv a0, %0" : : "r"(core_info->main_registers->a0));
  asm volatile ("mv a1, %0" : : "r"(core_info->main_registers->a1));
  asm volatile ("mv a2, %0" : : "r"(core_info->main_registers->a2));
  asm volatile ("mv a3, %0" : : "r"(core_info->main_registers->a3));
  asm volatile ("mv a4, %0" : : "r"(core_info->main_registers->a4));
  asm volatile ("mv a5, %0" : : "r"(core_info->main_registers->a5));
  asm volatile ("mv a6, %0" : : "r"(core_info->main_registers->a6));
  asm volatile ("mv ra, %0" : : "r"(core_info->main_registers->ra));
  asm volatile ("mv sp, %0" : : "r"(core_info->main_registers->sp));
  return;
}



void init_spec(){
    main_registers_t* main_registers = balloc(sizeof(main_registers_t));
    core_info_t* core_info = get_core_info_ptr();
    core_info->main_registers = main_registers;

}