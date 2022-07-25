export ARCH := mips-elf
export CFLAGS := -EB -ffreestanding -nostdlib -fno-PIC -mabi=32 -mno-shared -mno-abicalls -march=vr4300 -mtune=vr4300 -mfix4300 -G 0 -I $(realpath ../include) -MMD
export CC := $(ARCH)-gcc
export OBJCOPY := $(ARCH)-objcopy
export NM := $(ARCH)-nm
export BUILD_DIR := $(realpath ../build)

.PHONY: all
all: loader payload

.PHONY: payload
payload:
	$(MAKE) -C payload

.PHONY: loader
loader:
	$(MAKE) -C loader
