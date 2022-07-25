SRC 					:= loader.S
LDSCRIPT			:= ../link.ld.in

OOT_TARGET 		:= $(BUILD_DIR)/loader_oot
OOT_OBJ 			:= $(BUILD_DIR)/obj/loader/loader_oot.o
OOT_LDSCRIPT 	:= $(BUILD_DIR)/ld/loader_oot.ld

MM_TARGET 		:= $(BUILD_DIR)/loader_mm
MM_OBJ 				:= $(BUILD_DIR)/obj/loader/loader_mm.o
MM_LDSCRIPT 	:= $(BUILD_DIR)/ld/loader_mm.ld

.PHONY: all
all: $(OOT_TARGET) $(MM_TARGET)

-include $(shell find $(BUILD_DIR) -name '*.d')

$(OOT_TARGET): $(OOT_OBJ)
	@mkdir -p $(dir $@)
	$(OBJCOPY) -O binary $(OOT_OBJ) $@

$(OOT_OBJ): $(SRC) $(OOT_LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DGAME_OOT=1 -o $@ $(SRC) -T $(OOT_LDSCRIPT)

$(OOT_LDSCRIPT): $(LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) -E -P -x c -MMD -DGAME_OOT=1 -DLOADER=1 $< -o $@

$(MM_TARGET): $(MM_OBJ)
	@mkdir -p $(dir $@)
	$(OBJCOPY) -O binary $(MM_OBJ) $@

$(MM_OBJ): $(SRC) $(MM_LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DGAME_MM=1 -o $@ $(SRC) -T $(MM_LDSCRIPT)

$(MM_LDSCRIPT): $(LDSCRIPT)
	@mkdir -p $(dir $@)
	$(CC) -E -P -x c -MMD -DGAME_MM=1 -DLOADER=1 $< -o $@
