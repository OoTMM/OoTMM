ifneq (, $(shell command -v mips64-ultra-elf-gcc 2>/dev/null))
ARCH	:= mips64-ultra-elf
else ifneq (, $(shell command -v mips64-elf-gcc 2>/dev/null))
ARCH	:= mips64-elf
else ifneq (, $(shell command -v mips-elf-gcc 2>/dev/null))
ARCH	:= mips-elf
else
$(error "No MIPS toolchain found")
endif

CFLAGS 		:= -EB -ffreestanding -nostdlib -fno-PIC -mabi=32 -mno-shared -mno-abicalls \
			   -march=vr4300 -mtune=vr4300 -mfix4300 -G 0 -Os \
			   -isystem ../third_party/ultralib/include -isystem ../third_party/ultralib/include/gcc -I../include -I../build/include -MMD -MP \
			   -DF3DEX_GBI_2=1
CC 			:= $(ARCH)-gcc
OBJCOPY 	:= $(ARCH)-objcopy
NM 			:= $(ARCH)-nm
BUILD_DIR 	:= ../build

LDSCRIPT		:= link.ld.in
SRC				:= $(shell find payload/common -name '*.c' -o -name '*.S')

OOT_ELF			:= $(BUILD_DIR)/oot.elf
OOT_PAYLOAD		:= $(BUILD_DIR)/oot_payload.bin
OOT_PATCH		:= $(BUILD_DIR)/oot_patch.bin
OOT_SRC			:= $(shell find payload/oot -name '*.c' -o -name '*.S') $(SRC)
OOT_OBJ			:= $(OOT_SRC:%=$(BUILD_DIR)/obj/oot/%.o)
OOT_LDSCRIPT 	:= $(BUILD_DIR)/ld/oot.ld

MM_ELF			:= $(BUILD_DIR)/mm.elf
MM_PAYLOAD		:= $(BUILD_DIR)/mm_payload.bin
MM_PATCH		:= $(BUILD_DIR)/mm_patch.bin
MM_SRC			:= $(shell find payload/mm -name '*.c' -o -name '*.S') $(SRC)
MM_OBJ			:= $(MM_SRC:%=$(BUILD_DIR)/obj/mm/%.o)
MM_LDSCRIPT 	:= $(BUILD_DIR)/ld/mm.ld

CUSTOM_DATA    				:= $(BUILD_DIR)/custom.bin
CUSTOM_HEADER 				:= $(BUILD_DIR)/include/combo/custom.h
CUSTOM_OBJECTS_SRC 			:= $(shell find objects -name '*.c')
CUSTOM_OBJECTS_OBJ 			:= $(CUSTOM_OBJECTS_SRC:%.c=$(BUILD_DIR)/obj/%.o)
CUSTOM_OBJECTS_LDSCRIPT 	:= link_custom_object.ld

.PHONY: all
all: $(OOT_PAYLOAD) $(OOT_PATCH) $(MM_PAYLOAD) $(MM_PATCH) $(CUSTOM_DATA)

-include $(shell find $(BUILD_DIR) -name '*.d')

$(OOT_PAYLOAD): $(OOT_ELF)
	@mkdir -p $(dir $@)
	$(OBJCOPY) --only-section=.text -O binary $< $@

$(OOT_PATCH): $(OOT_ELF)
	@mkdir -p $(dir $@)
	$(OBJCOPY) --only-section=.patch -O binary $< $@

$(OOT_ELF): $(OOT_OBJ) $(OOT_LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(OOT_OBJ) -T $(OOT_LDSCRIPT) -o $@

$(BUILD_DIR)/obj/oot/%.S.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DGAME_OOT=1 -c -o $@ $<

$(BUILD_DIR)/obj/oot/%.c.o: %.c | $(CUSTOM_HEADER)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DGAME_OOT=1 -c -o $@ $<

$(OOT_LDSCRIPT): $(LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) -I../include -E -P -x c -MMD -MT $@ -DGAME_OOT=1 $< -o $@

$(MM_PAYLOAD): $(MM_ELF)
	@mkdir -p $(dir $@)
	$(OBJCOPY) --only-section=.text -O binary $< $@

$(MM_PATCH): $(MM_ELF)
	@mkdir -p $(dir $@)
	$(OBJCOPY) --only-section=.patch -O binary $< $@

$(MM_ELF): $(MM_OBJ) $(MM_LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(MM_OBJ) -T $(MM_LDSCRIPT) -o $@

$(BUILD_DIR)/obj/mm/%.S.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DGAME_MM=1 -c -o $@ $<

$(BUILD_DIR)/obj/mm/%.c.o: %.c | $(CUSTOM_HEADER)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DGAME_MM=1 -c -o $@ $<

$(MM_LDSCRIPT): $(LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) -I../include -E -P -x c -MMD -MT $@ -DGAME_MM=1 $< -o $@

$(BUILD_DIR)/obj/objects/%.o: objects/%.c $(CUSTOM_OBJECTS_LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -fno-toplevel-reorder -T$(CUSTOM_OBJECTS_LDSCRIPT) -o $@ $<

$(CUSTOM_DATA) $(CUSTOM_HEADER) &: $(CUSTOM_OBJECTS_OBJ)
	@mkdir -p $(BUILD_DIR)/include/combo
	ruby ../bin/combo --generate-custom

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/obj $(BUILD_DIR)/ld
