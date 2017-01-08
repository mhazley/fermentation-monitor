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
TARGET_FILE=brewbot-photon
TARGET_DIR=../../../build/

APP=brewbot-$(PLATFORM)

STATE=$(shell cat $(STATE_FILE))

# BREWBOT_DEBUG SETUP
ifeq ("$(BREWBOT_DEBUG)","y")
CPPFLAGS_EX+=-DBREWBOT_DEBUG
# Logical OR in a makefile, filter returns empty string if not equal to either option
else ifneq (,$(filter $(MAKECMDGOALS),debug, program-debug))
CPPFLAGS_EX+=-DBREWBOT_DEBUG
endif

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

# CHECK FOR HW REVISION IN ENV
ifeq (,$(filter $(MAKECMDGOALS), clean))
ifeq (,$(filter $(HW_REV),A B))
$(error HW_REV is undefined)
endif
endif

# CHECKING FOR BETA BUILD - REMOVE SECURITY KEY IF SO
ifneq (,$(filter $(MAKECMDGOALS), beta))
CPPFLAGS_EX+=-DDISABLE_SECURITY
endif

# CHECK FOR ENV VARIABLES TO EFFECT BUILD
ifeq ("$(HW_REV)","A")
CPPFLAGS_EX+=-DHW_REV_A
endif

ifeq ("$(HW_REV)","B")
CPPFLAGS_EX+=-DHW_REV_B
endif

ifeq ("$(STATIC_BATCH)","y")
CPPFLAGS_EX+=-DSTATIC_BATCH
endif

ifeq ("$(VERBOSE_SERIAL)","y")
CPPFLAGS_EX+=-DVERBOSE_SERIAL
endif

ifeq ("$(TRI_STATE_HEATER)","y")
CPPFLAGS_EX+=-DTRI_STATE_HEATER
endif

ifeq ("$(NO_STAGE_WAIT)","y")
CPPFLAGS_EX+=-DNO_STAGE_WAIT
endif

ifeq ("$(LOGGER_DEBUG)","y")
CPPFLAGS_EX+=-DLOGGER_DEBUG
endif

ifeq ("$(DISABLE_CONCURRENT_STAGES)","y")
CPPFLAGS_EX+=-DDISABLE_CONCURRENT_STAGES
endif

ifeq ("$(DISABLE_PWM)","y")
CPPFLAGS_EX+=-DDISABLE_PWM
endif

ifeq ("$(STATIC_BATCH_WITH_CLEAN)","y")
CPPFLAGS_EX+=-DSTATIC_BATCH_WITH_CLEAN
endif

ifeq ("$(PINMAPPING_OLD)","y")
CPPFLAGS_EX+=-DPINMAPPING_OLD
else ifneq (,$(filter $(MAKECMDGOALS),debug, program-debug))
CPPFLAGS_EX+=-DPINMAPPING_OLD
endif

ifeq ("$(DISABLE_SECURITY)","y")
CPPFLAGS_EX+=-DDISABLE_SECURITY
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

