#ifndef S_TRAP_H
#define S_TRAP_H

struct context {
  unsigned long ra;
  unsigned long sp;
  unsigned long s0;
  unsigned long s1;
  unsigned long s2;
  unsigned long s3;
  unsigned long s4;
  unsigned long s5;
  unsigned long s6;
  unsigned long s7;
  unsigned long s8;
  unsigned long s9;
  unsigned long s10;
  unsigned long s11;
} __attribute__((packed));

extern __attribute__((awlays_inline)) inline void s_trap();
extern __attribute__((always_inline)) inline void switch_context(struct context* old,struct context* nw);

#endif  //S_TRAP_H
