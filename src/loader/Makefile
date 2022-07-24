TARGET_OOT := $(BUILD_DIR)/loader_oot
TARGET_OOT_SRC := loader_oot.S
TARGET_OOT_OBJ := $(BUILD_DIR)/obj/loader/loader_oot.o
TARGET_OOT_LDSCRIPT := loader_oot.ld

.PHONY: all
all: $(TARGET_OOT)

$(TARGET_OOT): $(TARGET_OOT_OBJ)
	@mkdir -p $(dir $@)
	$(OBJCOPY) -O binary $(TARGET_OOT_OBJ) $@

$(TARGET_OOT_OBJ): $(TARGET_OOT_SRC) $(TARGET_OOT_LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $(TARGET_OOT_SRC) -T $(TARGET_OOT_LDSCRIPT)
