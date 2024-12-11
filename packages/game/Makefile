MAKE := make
MAKEARGS := -j

OOT_VERSION := ntsc-1.0
OOT_REGION 	:= US

MM_VERSION 	:= n64-us

ROMS_DIR        := ../../roms

LIBS_DIR 		:= libs
LIBS_DIR_OOT 	:= $(LIBS_DIR)/oot
LIBS_DIR_MM 	:= $(LIBS_DIR)/mm

MAKEARGS_OOT := COMPARE=0 NON_MATCHING=1 AVOID_UB=1 VERSION=$(OOT_VERSION) REGION=$(OOT_REGION)
MAKEARGS_MM  := COMPARE=0 NON_MATCHING=1 AVOID_UB=1 VERSION=$(MM_VERSION)

.PHONY: all
all: oot mm

.PHONY: setup
setup: setup-oot setup-mm

.PHONY: setup-oot
setup-oot:
	ln -sf $(realpath $(ROMS_DIR)/oot.z64) $(LIBS_DIR_OOT)/baseroms/$(OOT_VERSION)/baserom.z64
	$(MAKE) $(MAKEARGS) $(MAKEARGS_OOT) -C $(LIBS_DIR_OOT) setup -j

.PHONY: setup-mm
setup-mm:
	ln -sf $(realpath $(ROMS_DIR)/mm.z64) $(LIBS_DIR_MM)/baseroms/$(MM_VERSION)/baserom.z64
	$(MAKE) $(MAKEARGS) $(MAKEARGS_MM) -C $(LIBS_DIR_MM) venv -j
	$(MAKE) $(MAKEARGS) $(MAKEARGS_MM) -C $(LIBS_DIR_MM) setup -j
	$(MAKE) $(MAKEARGS) $(MAKEARGS_MM) -C $(LIBS_DIR_MM) assets -j
	$(MAKE) $(MAKEARGS) $(MAKEARGS_MM) -C $(LIBS_DIR_MM) disasm -j

.PHONY: oot
oot:
	$(MAKE) $(MAKEARGS) $(MAKEARGS_OOT) -C $(LIBS_DIR_OOT) -j

.PHONY: mm
mm:
	$(MAKE) $(MAKEARGS) $(MAKEARGS_MM) -C $(LIBS_DIR_MM) -j

.PHONY: clean
clean:
	rm -rf $(LIBS_DIR_OOT)/build
	rm -rf $(LIBS_DIR_MM)/build