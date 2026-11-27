
qemu-system-riscv64 \
  -bios default \
  -m 256M \
  -kernel ./bin/OS.elf \
  -nographic \
  -machine sifive_u\
  -smp 2 