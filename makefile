CSRC_C := $(shell find ./ -name "*.c")
CSRC_S := $(shell find ./ -name "*.s") 
CSRC   := $(CSRC_C) $(CSRC_S)

CTAR := $(patsubst %.c, bin/%.o, $(notdir $(CSRC_C))) \
        $(patsubst %.s, bin/%.o, $(notdir $(CSRC_S)))

CFLG := -march=rv64gc -mabi=lp64 -nostdinc -nostdlib -Winline -fPIC \
        -fdebug-prefix-map=$(CURDIR)=. -ffile-prefix-map=$(CURDIR)=. \
        -mno-relax -mcmodel=medany -O3 

CC := clang
INCDIRS := -I$(CURDIR)/include -I$(CURDIR)/include/bsp/sifive_fu540
AR := llvm-ar

.PHONY: all clean ecomp comp

all: ecomp comp

ecomp:
	@clear
	@mkdir -p bin

VPATH := $(sort $(dir $(CSRC)))

bin/%.o: %.c
	$(CC) --target=riscv64-unknown-elf $(CFLG)-D DTB $(INCDIRS) -c $< -o $@

bin/%.o: %.s
	$(CC) --target=riscv64-unknown-elf $(CFLG)-D DTB $(INCDIRS) -c $< -o $@


comp: $(CTAR)
	@echo "Objects successfully compiled in folder bin"
	$(AR) rcs rv64_sifive_hal.a $(CTAR)

clean:
	rm -rf bin rv64_sifive_hal.a
