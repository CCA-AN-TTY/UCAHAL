.section .text
.globl _start

_start:
  
  mv t0,a0
  la t2,_stack_top
  li t3,0x2000

  
  mul t1,t0,t3
  sub t2,t2,t1
  mv sp,t2


  la t0,is_main
  lw t2,0(t0)
  li t1,1
  beq t2,t1,other_way
  
  li t3,1
  sw t3,0(t0)
  
  j hal_init

other_way:
  j hart_main
  


.section .data
  is_main: .word 0x00

.section .bss
.align 16
.space 4096*10
_stack_top: