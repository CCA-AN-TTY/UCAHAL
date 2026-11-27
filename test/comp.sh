set -e
cd .. 
make
echo "H"
cd test

mkdir -p bin


make 



echo "LINK"
ld.lld -mno-relax -m elf64lriscv -T linker.ld --start-group  ../bin/boot.o ../rv64_sifive_hal.a \
rv64_kernel.a --end-group \
-o ./bin/OS.elf
