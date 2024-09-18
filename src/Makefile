SHELL=/bin/bash
ROOT_DIR:=$(shell pwd)

export PATH := $(PATH)

CD       := cd
CM4_DIR  := $(ROOT_DIR)/app_cm4
CM0_DIR  := $(ROOT_DIR)/app_cm0p
RM	 := rm -rf

ifeq (,$(findstring clean,$(MAKECMDGOALS)))
XR_VER=$(shell cd ../..; git describe --always --dirty)
VER=$(shell git describe --always --dirty)
HN:=$(shell hostname -f)
COMPILER_VER:=${CY_TOOLS_PATHS}
# FIXME JS 3/6/23 Above only shows path not actual compiler version,
# would be nice to have something similar to below instead.
#COMPILER_VER:=$(shell $(CC) --version | head -1)
LINKER_VER:=$(shell $(LD) --version | head -1)
endif


cleancm0:
	@echo "--------| Cleaning M0 area"
	export CY_TOOLS_PATHS=/opt/ModusToolbox/tools_3.0/; \
	$(RM) $(CM0_DIR)/build/; \
	$(MAKE) -C $(CM0_DIR) clean;

cleancm4:
	@echo "--------| Cleaning M4 area"
	export CY_TOOLS_PATHS=/opt/ModusToolbox/tools_3.0/; \
	$(RM) $(CM4_DIR)/build/; \
	$(MAKE) -C $(CM4_DIR) clean;

cleanalldir: cleancm0 cleancm4

cleandir: cleancm0 cleancm4
