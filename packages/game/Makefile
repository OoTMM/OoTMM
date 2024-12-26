MAKE := make
MAKEARGS ?= -j

OOT_VERSION := ntsc-1.0
OOT_REGION 	:= US

MM_VERSION 	:= n64-us

ROMS_DIR        := ../../roms

LIBS_DIR 			:= libs
LIBS_DIR_OOT 		:= $(LIBS_DIR)/oot
LIBS_DIR_MM 		:= $(LIBS_DIR)/mm
LIBS_DIR_LOADER 	:= $(LIBS_DIR)/loader

MAKEARGS_OOT := COMPILER=gcc COMPARE=0 NON_MATCHING=1 AVOID_UB=1 VERSION=$(OOT_VERSION) REGION=$(OOT_REGION)
MAKEARGS_MM  := COMPILER=gcc COMPARE=0 NON_MATCHING=1 AVOID_UB=1 VERSION=$(MM_VERSION)

.PHONY: all
all: oot mm

.PHONY: setup
setup: setup-oot setup-mm

.PHONY: setup-oot
setup-oot: stamps/setup-oot

stamps/setup-oot:
	@mkdir -p stamps
	ln -sf $(realpath $(ROMS_DIR)/oot.z64) $(LIBS_DIR_OOT)/baseroms/$(OOT_VERSION)/baserom.z64
	$(MAKE) $(MAKEARGS) $(MAKEARGS_OOT) -C $(LIBS_DIR_OOT) setup
	touch $@

.PHONY: setup-mm
setup-mm: stamps/setup-mm

stamps/setup-mm:
	@mkdir -p stamps
	ln -sf $(realpath $(ROMS_DIR)/mm.z64) $(LIBS_DIR_MM)/baseroms/$(MM_VERSION)/baserom.z64
	$(MAKE) $(MAKEARGS) $(MAKEARGS_MM) -C $(LIBS_DIR_MM) venv
	$(MAKE) $(MAKEARGS) $(MAKEARGS_MM) -C $(LIBS_DIR_MM) setup
	$(MAKE) $(MAKEARGS) $(MAKEARGS_MM) -C $(LIBS_DIR_MM) assets
	$(MAKE) $(MAKEARGS) $(MAKEARGS_MM) -C $(LIBS_DIR_MM) disasm
	touch $@

.PHONY: oot
oot: setup loader
	@mkdir -p build
	$(MAKE) $(MAKEARGS) $(MAKEARGS_OOT) -C $(LIBS_DIR_OOT)
	tsx ./spec2fs.ts $(LIBS_DIR_OOT)/build/$(OOT_VERSION)/spec $(LIBS_DIR_OOT)/build/$(OOT_VERSION)/oot-$(OOT_VERSION).elf dist/oot.json
	cp $(LIBS_DIR_OOT)/build/$(OOT_VERSION)/oot-$(OOT_VERSION).z64 dist/oot.z64

.PHONY: mm
mm: setup loader
	$(MAKE) $(MAKEARGS) $(MAKEARGS_MM) -C $(LIBS_DIR_MM)
	tsx ./spec2fs.ts $(LIBS_DIR_MM)/spec $(LIBS_DIR_MM)/build/$(MM_VERSION)/mm-$(MM_VERSION).elf dist/mm.json
	cp $(LIBS_DIR_MM)/build/$(MM_VERSION)/mm-$(MM_VERSION).z64 dist/mm.z64

.PHONY: loader
loader: setup
	$(MAKE) $(MAKEARGS) -C $(LIBS_DIR_LOADER)

.PHONY: clean
clean:
	rm -rf $(LIBS_DIR_OOT)/build
	rm -rf $(LIBS_DIR_MM)/build
	rm -rf $(LIBS_DIR_LOADER)/build

.PHONY: distclean
distclean: clean
	$(MAKE) $(MAKEARGS) -C $(LIBS_DIR_OOT) distclean
	$(MAKE) $(MAKEARGS) -C $(LIBS_DIR_MM) distclean

.PHONY: run-oot
run-oot:
	"$(N64_EMULATOR)" $(LIBS_DIR_OOT)/build/$(OOT_VERSION)/oot-$(OOT_VERSION).z64

.PHONY: run-mm
run-mm:
	"$(N64_EMULATOR)" $(LIBS_DIR_MM)/build/$(MM_VERSION)/mm-$(MM_VERSION).z64
