PARTICLE_BIN=node_modules/.bin/particle
STATE_FILE=debug/state.txt

ifneq ("$(CONFIG_FILE)",)
include $(CONFIG_FILE)
endif

# SET UP THE BUILD SHA's FOR INCLUSION
BUILD_COMMIT=$(shell git rev-parse --short HEAD)
PARTICLE_COMMIT=$(shell cd particle/firmware; git rev-parse --short HEAD)
DIRTY_FILES=$(shell git status --porcelain 2>/dev/null| grep "^ M" | wc -l)

# SET UP SRC & BUILD DIRs, SET TARGET
APPDIR=../../../src
PLATFORM?=photon
TARGET_FILE=fermentation-photon
TARGET_DIR=../../../build/

APP=fermentation-$(PLATFORM)

STATE=$(shell cat $(STATE_FILE))

# CHECKING FOR DEBUG BUILD - SET CLEAN FLAG IF SO
ifneq (,$(filter $(MAKECMDGOALS), debug, program-debug))
ifneq ("$(STATE)", "debug")
NEED_TO_CLEAN=clean
endif
$(shell rm $(STATE_FILE))
$(shell echo debug > $(STATE_FILE))
else
ifneq ("$(STATE)", "normal")
NEED_TO_CLEAN=clean
endif
$(shell rm $(STATE_FILE))
$(shell echo normal > $(STATE_FILE))
endif

CPPFLAGS_EX+="-DBUILD_SHA=$(BUILD_COMMIT)"
CPPFLAGS_EX+="-DPARTICLE_SHA=$(PARTICLE_COMMIT)"

ifeq ($(shell test $(DIRTY_FILES) -gt 0; echo $$?),0)
	CPPFLAGS_EX+=-DDIRTY_TREE
endif

export CPPFLAGS=$(CPPFLAGS_EX)

all elf bin hex size clean:
	@$(MAKE) -C particle/firmware/main PLATFORM=$(PLATFORM) APP=$(APP) APPDIR=$(APPDIR) TARGET_DIR=$(TARGET_DIR) TARGET_FILE=$(TARGET_FILE) v=1 $@

beta:
	@$(MAKE) -C particle/firmware/main PLATFORM=$(PLATFORM) APP=$(APP) APPDIR=$(APPDIR) TARGET_DIR=$(TARGET_DIR) TARGET_FILE=$(TARGET_FILE) v=1 all

debug:
	@$(MAKE) -C particle/firmware/main PLATFORM=$(PLATFORM) APP=$(APP) APPDIR=$(APPDIR) TARGET_DIR=$(TARGET_DIR) TARGET_FILE=$(TARGET_FILE) USE_SWD_JTAG=y MODULAR=n v=1 $(NEED_TO_CLEAN) all

program-with-ssid:
	@$(MAKE) -C particle/firmware/main PLATFORM=$(PLATFORM) APP=$(APP) APPDIR=$(APPDIR) TARGET_DIR=$(TARGET_DIR) TARGET_FILE=$(TARGET_FILE) v=1 $(NEED_TO_CLEAN) all program-dfu
	./tools/set_ssid.sh

program:
	@$(MAKE) -C particle/firmware/main PLATFORM=$(PLATFORM) APP=$(APP) APPDIR=$(APPDIR) TARGET_DIR=$(TARGET_DIR) TARGET_FILE=$(TARGET_FILE) v=1 $(NEED_TO_CLEAN) all program-dfu

program-debug:
	@$(MAKE) -C particle/firmware/main PLATFORM=$(PLATFORM) APP=$(APP) APPDIR=$(APPDIR) TARGET_DIR=$(TARGET_DIR) TARGET_FILE=$(TARGET_FILE) USE_SWD_JTAG=y MODULAR=n v=1 $(NEED_TO_CLEAN) all program-dfu

program-particle-debug:
	@$(MAKE) -C particle/firmware/main PLATFORM=$(PLATFORM) APP=$(APP) APPDIR=$(APPDIR) TARGET_DIR=$(TARGET_DIR) TARGET_FILE=$(TARGET_FILE) DEBUG_BUILD=y v=1 $(NEED_TO_CLEAN) all program-dfu

upgrade:
	@$(MAKE) -C particle/firmware/modules PLATFORM=$(PLATFORM) APP=$(APP) APPDIR=$(APPDIR) TARGET_DIR=$(TARGET_DIR) TARGET_FILE=$(TARGET_FILE) v=1 clean all program-dfu

wifi:
	@$(PARTICLE_BIN) setup

system:
	rm -rf build/*
	@$(MAKE) -C particle/firmware/modules PLATFORM=$(PLATFORM) APP=$(APP) APPDIR=$(APPDIR) TARGET_DIR=$(TARGET_DIR) TARGET_FILE=$(TARGET_FILE) v=1 clean all
	cp particle/firmware/build/brewbot-photon.bin build/
	cp particle/firmware/build/target/system-part1/platform-6-m/system-part1.bin build/
	cp particle/firmware/build/target/system-part2/platform-6-m/system-part2.bin build/
