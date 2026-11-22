.macro ctx_sv base
  sd ra,0(\base)
  sd sp,0x8(\base)
  sd s0,0x10(\base)
  sd s1,0x18(\base)
  sd s2,0x20(\base)
  sd s3,0x28(\base)
  sd s4,0x30(\base)
  sd s5,0x38(\base)
  sd s6,0x40(\base)
  sd s7,0x48(\base)
  sd s8,0x50(\base)
  sd s9,0x58(\base)
  sd s10,0x60(\base)
  sd s11,0x68(\base)
.endm

.macro ctx_ld base
  ld ra,0(\base)
  ld sp,0x8(\base)
  ld s0,0x10(\base)
  ld s1,0x18(\base)
  ld s2,0x20(\base)
  ld s3,0x28(\base)
  ld s4,0x30(\base)
  ld s5,0x38(\base)
  ld s6,0x40(\base)
  ld s7,0x48(\base)
  ld s8,0x50(\base)
  ld s9,0x58(\base)
  ld s10,0x60(\base)
  ld s11,0x68(\base)
.endm


.globl switch_context
.globl s_trap
.align 16

s_trap:
  csrr a0,sepc
  csrr a1,scause
  csrr a2,stval

  call trap_hand

  csrw sepc,a0
  sret

.align 16
switch_context:
  ctx_sv a0 #old one
  ctx_ld a1 #new one
  jr ra


