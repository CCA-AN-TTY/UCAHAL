#include "arch/rv64/drivers/sbi/sbi.h"


inline long return_val(sbi_ret_t sbi_ret_t){
  return sbi_ret_t.value;
}
inline long return_error(sbi_ret_t sbi_ret_t){
  return sbi_ret_t.error;
}

sbi_ret_t sbi_call(sbi_args_t* a){
  register unsigned long a0 asm("a0") = a->a0;
  register unsigned long a1 asm("a1") = a->a1;
  register unsigned long a2 asm("a2") = a->a2;
  register unsigned long a3 asm("a3") = a->a3;
  register unsigned long a4 asm("a4") = a->a4;
  register unsigned long a5 asm("a5") = a->a5;
  register unsigned long a6 asm("a6") = a->FID;
  register unsigned long a7 asm("a7") = a->EID;


  asm volatile (
    "ecall"
    : "+r" (a0), "+r" (a1)
    : "r" (a2), "r" (a3), "r" (a4), "r" (a5), "r" (a6), "r" (a7)
    : "memory" 
  );

  sbi_ret_t result;
  result.error = (sbi_error_t)a0;
  result.value = (long)a1;
  return result;

}