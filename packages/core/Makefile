# Configurable
DEBUG ?= 0

# Toolchain detection
ifneq (, $(shell command -v mips64-ultra-elf-gcc 2>/dev/null))
ARCH	:= mips64-ultra-elf
else ifneq (, $(shell command -v mips64-elf-gcc 2>/dev/null))
ARCH	:= mips64-elf
else ifneq (, $(shell command -v mips-elf-gcc 2>/dev/null))
ARCH	:= mips-elf
else ifneq (, $(shell command -v mips64-ultra-gcc 2>/dev/null))
ARCH	:= mips64-ultra
else ifneq (, $(shell command -v mips64-gcc 2>/dev/null))
ARCH	:= mips64
else ifneq (, $(shell command -v mips-gcc 2>/dev/null))
ARCH	:= mips
else
$(error "No MIPS toolchain found")
endif

CFLAGS 		:= -EB -ffreestanding -nostdlib -fno-PIC -mabi=32 -mno-shared -mno-abicalls \
			   -march=vr4300 -mtune=vr4300 -mfix4300 -mfix-r4000 -G 0 -Os \
			   -Wall -Werror=implicit-function-declaration -Werror=implicit-int -Wimplicit-fallthrough \
			   -isystem third_party/ultralib/include -isystem third_party/ultralib/include/gcc -Iinclude -Ibuild/include \
			    -MMD -MP \
			   -DF3DEX_GBI_2=1
CC 				:= $(ARCH)-gcc
OBJCOPY 		:= $(ARCH)-objcopy
NM 				:= $(ARCH)-nm
BUILD_DIR_ROOT 	:= build
SRC_DIR 		:= src
DEBUG_FILE  	:= include/combo/debug.h

# Debug/Release
ifeq ($(DEBUG), 0)
BUILD_DIR 	:= $(BUILD_DIR_ROOT)/Release
else
BUILD_DIR 	:= $(BUILD_DIR_ROOT)/Debug
CFLAGS 		+= -DDEBUG=1 -g3
endif

LDSCRIPT		:= $(SRC_DIR)/link.ld.in
SRC				:= $(shell find $(SRC_DIR)/common $(BUILD_DIR_ROOT)/src/common -name '*.c' -o -name '*.S')

OOT_ELF			:= $(BUILD_DIR)/oot.elf
OOT_PAYLOAD		:= $(BUILD_DIR)/oot_payload.bin
OOT_PATCH		:= $(BUILD_DIR)/oot_patch.bin
OOT_SRC			:= $(shell find $(SRC_DIR)/oot -name '*.c' -o -name '*.S') $(SRC)
OOT_OBJ			:= $(OOT_SRC:%=$(BUILD_DIR)/obj/oot/%.o)
OOT_LDSCRIPT 	:= $(BUILD_DIR)/ld/oot.ld

MM_ELF			:= $(BUILD_DIR)/mm.elf
MM_PAYLOAD		:= $(BUILD_DIR)/mm_payload.bin
MM_PATCH		:= $(BUILD_DIR)/mm_patch.bin
MM_SRC			:= $(shell find $(SRC_DIR)/mm -name '*.c' -o -name '*.S') $(SRC)
MM_OBJ			:= $(MM_SRC:%=$(BUILD_DIR)/obj/mm/%.o)
MM_LDSCRIPT 	:= $(BUILD_DIR)/ld/mm.ld

.PHONY: all
all: $(OOT_PAYLOAD) $(OOT_PATCH) $(MM_PAYLOAD) $(MM_PATCH)

-include $(shell find $(BUILD_DIR) -name '*.d' 2>/dev/null)

$(OOT_PAYLOAD): $(OOT_ELF)
	@mkdir -p $(dir $@)
	$(OBJCOPY) --only-section=.text -O binary $< $@

$(OOT_PATCH): $(OOT_ELF)
	@mkdir -p $(dir $@)
	$(OBJCOPY) --only-section=.patch -O binary $< $@

$(OOT_ELF): $(OOT_OBJ) $(OOT_LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(OOT_OBJ) -T $(OOT_LDSCRIPT) -o $@

$(BUILD_DIR)/obj/oot/%.S.o: %.S $(DEBUG_FILE)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DGAME_OOT=1 -c -o $@ $<

$(BUILD_DIR)/obj/oot/%.c.o: %.c $(DEBUG_FILE)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -D_LANGUAGE_C=1 -DGAME_OOT=1 -c -o $@ $<

$(OOT_LDSCRIPT): $(LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) -Iinclude -E -P -x c -MMD -MT $@ -DGAME_OOT=1 $< -o $@

$(MM_PAYLOAD): $(MM_ELF)
	@mkdir -p $(dir $@)
	$(OBJCOPY) --only-section=.text -O binary $< $@

$(MM_PATCH): $(MM_ELF)
	@mkdir -p $(dir $@)
	$(OBJCOPY) --only-section=.patch -O binary $< $@

$(MM_ELF): $(MM_OBJ) $(MM_LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(MM_OBJ) -T $(MM_LDSCRIPT) -o $@

$(BUILD_DIR)/obj/mm/%.S.o: %.S $(DEBUG_FILE)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DGAME_MM=1 -c -o $@ $<

$(BUILD_DIR)/obj/mm/%.c.o: %.c $(DEBUG_FILE)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -D_LANGUAGE_C=1 -DGAME_MM=1 -c -o $@ $<

$(MM_LDSCRIPT): $(LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) -Iinclude -E -P -x c -MMD -MT $@ -DGAME_MM=1 $< -o $@

$(DEBUG_FILE):
	touch $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/obj $(BUILD_DIR)/ld
