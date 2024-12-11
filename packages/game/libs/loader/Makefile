ARCH 	:= mips-linux-gnu
CC 		:= $(ARCH)-gcc
LD    	:= $(ARCH)-ld

CFLAGS      = 	-mips3 -G 0 -nostdinc -march=vr4300 -mfix4300 -mabi=32 \
				-mno-abicalls -mdivide-breaks -fno-PIC -fno-common \
				-ffreestanding -nostdlib -fbuiltin -fno-builtin-sinf -fno-builtin-cosf -funsigned-char \
				$(CPP_DEFINES) $(INC) $(OPTFLAGS)

CPP_DEFINES := -DOOT_VERSION=NTSC_1_0 -DOOT_REGION=REGION_US -DPLATFORM_N64=1 -DAVOID_UB=1 -DNON_MATCHING=1
OPTFLAGS   	:= -O2
INC         :=

SRC_DIR 	:= src
BUILD_DIR 	:= build
OBJ_DIR 	:= $(BUILD_DIR)/obj
LDSCRIPT 	:= loader.ld

SRC := $(shell find $(SRC_DIR) -name "*.c" -o -name "*.s" -o -name "*.S")
OBJ := $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(SRC:%=%.o))

TARGET := $(BUILD_DIR)/loader.o

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJ) $(LDSCRIPT)
	$(LD) -r -o $@ $(OBJ) -T $(LDSCRIPT)

$(OBJ_DIR)/%.s.o: $(SRC_DIR)/%.s
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.S.o: $(SRC_DIR)/%.S
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
