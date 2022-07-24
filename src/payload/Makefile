TARGET_OOT := $(BUILD_DIR)/payload_oot
TARGET_OOT_ELF := $(TARGET_OOT).elf
TARGET_OOT_SYM := $(TARGET_OOT).sym
TARGET_OOT_SRC := $(wildcard *.S)
TARGET_OOT_OBJ := $(TARGET_OOT_SRC:%=$(BUILD_DIR)/obj/payload/%.o)
TARGET_OOT_LDSCRIPT := payload.ld

.PHONY: all
all: $(TARGET_OOT)

$(TARGET_OOT): $(TARGET_OOT_ELF)
	@mkdir -p $(dir $@)
	$(NM) -f bsd --defined-only $< > $(TARGET_OOT_SYM)
	$(OBJCOPY) -O binary $< $@

$(TARGET_OOT_ELF): $(TARGET_OOT_OBJ) $(TARGET_OOT_LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(TARGET_OOT_OBJ) -T $(TARGET_OOT_LDSCRIPT) -o $@

$(BUILD_DIR)/obj/payload/%.S.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<
