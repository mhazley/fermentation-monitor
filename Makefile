# SET UP THE BUILD SHA's FOR INCLUSION
BUILD_COMMIT=$(shell git rev-parse --short HEAD)
PARTICLE_COMMIT=$(shell cd particle/firmware; git rev-parse --short HEAD)

# SET UP SRC & BUILD DIRs, SET TARGET
APPDIR=../../../src
PLATFORM?=electron
TARGET_FILE=geometry-fermenter
TARGET_DIR=../../../build/

APP=geometry-fermenter$(PLATFORM)

CPPFLAGS_EX+="-DBUILD_SHA=$(BUILD_COMMIT)"
CPPFLAGS_EX+="-DPARTICLE_SHA=$(PARTICLE_COMMIT)"
CPPFLAGS_EX+="-DARDUINO=100"

all elf bin hex size clean:
	@$(MAKE) -C particle/firmware/main PLATFORM=$(PLATFORM) APP=$(APP) APPDIR=$(APPDIR) TARGET_DIR=$(TARGET_DIR) TARGET_FILE=$(TARGET_FILE) v=1 $@

program:
	@$(MAKE) -C particle/firmware/main PLATFORM=$(PLATFORM) APP=$(APP) APPDIR=$(APPDIR) TARGET_DIR=$(TARGET_DIR) TARGET_FILE=$(TARGET_FILE) v=1 $(NEED_TO_CLEAN) all program-dfu

upgrade:
	@$(MAKE) -C particle/firmware/modules PLATFORM=$(PLATFORM) APP=$(APP) APPDIR=$(APPDIR) TARGET_DIR=$(TARGET_DIR) TARGET_FILE=$(TARGET_FILE) v=1 all program-dfu
