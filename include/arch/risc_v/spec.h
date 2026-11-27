#ifndef SPEC_H
#define SPEC_H

#include "hal_types.h"
typedef struct{
  reg_t a0;
  reg_t a1;
  reg_t a2;
  reg_t a3;
  reg_t a4;
  reg_t a5;
  reg_t a6;
  reg_t ra;
  reg_t sp;
}main_registers_t;

typedef struct core_info_t core_info_t;

extern inline void reg_snapshot();
extern inline uintn_t hal_get_core_id();
extern inline core_info_t* get_core_info_ptr();
extern inline void set_core_info_ptr(core_info_t* core_info);
extern void init_spec();

#endif //SPEC_H