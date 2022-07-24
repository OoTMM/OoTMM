export ARCH := mips-elf
export CFLAGS := -EB -mips64 -ffreestanding -nostdlib -I $(realpath ../include)
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
