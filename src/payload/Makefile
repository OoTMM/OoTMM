LDSCRIPT      := ../link.ld.in
SRC 					:= $(shell find common -name '*.c' -o -name '*.S')

OOT_TARGET 		:= $(BUILD_DIR)/payload_oot
OOT_ELF 			:= $(OOT_TARGET).elf
OOT_SYM 			:= $(OOT_TARGET).sym
OOT_SRC 			:= $(shell find oot -name '*.c' -o -name '*.S') $(SRC)
OOT_OBJ 			:= $(OOT_SRC:%=$(BUILD_DIR)/obj/payload_oot/%.o)
OOT_LDSCRIPT 	:= $(BUILD_DIR)/ld/payload_oot.ld

MM_TARGET 		:= $(BUILD_DIR)/payload_mm
MM_ELF 				:= $(MM_TARGET).elf
MM_SYM 				:= $(MM_TARGET).sym
MM_SRC 				:= $(shell find mm -name '*.c' -o -name '*.S') $(SRC)
MM_OBJ 				:= $(MM_SRC:%=$(BUILD_DIR)/obj/payload_mm/%.o)
MM_LDSCRIPT 	:= $(BUILD_DIR)/ld/payload_mm.ld

.PHONY: all
all: $(OOT_TARGET) $(MM_TARGET)

-include $(shell find $(BUILD_DIR) -name '*.d')

$(OOT_TARGET): $(OOT_ELF)
	@mkdir -p $(dir $@)
	$(NM) -f bsd --defined-only $< > $(OOT_SYM)
	$(OBJCOPY) -O binary $< $@

$(OOT_ELF): $(OOT_OBJ) $(OOT_LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(OOT_OBJ) -T $(OOT_LDSCRIPT) -o $@

$(BUILD_DIR)/obj/payload_oot/%.S.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DGAME_OOT=1 -c -o $@ $<

$(BUILD_DIR)/obj/payload_oot/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DGAME_OOT=1 -c -o $@ $<

$(OOT_LDSCRIPT): $(LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) -E -P -x c -MMD -DGAME_OOT=1 $< -o $@

$(MM_TARGET): $(MM_ELF)
	@mkdir -p $(dir $@)
	$(NM) -f bsd --defined-only $< > $(MM_SYM)
	$(OBJCOPY) -O binary $< $@

$(MM_ELF): $(MM_OBJ) $(MM_LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(MM_OBJ) -T $(MM_LDSCRIPT) -o $@

$(BUILD_DIR)/obj/payload_mm/%.S.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DGAME_MM=1 -c -o $@ $<

$(BUILD_DIR)/obj/payload_mm/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DGAME_MM=1 -c -o $@ $<

$(MM_LDSCRIPT): $(LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) -E -P -x c -MMD -DGAME_MM=1 $< -o $@
