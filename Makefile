SHELL=/bin/bash
ROOT_DIR:=$(shell pwd)

export PATH := $(PATH)

# paths
DEPLOY_DIR := $(ROOT_DIR)/deploy
INSTALL_DIR=$(ROOT_DIR)/image
INSTALL_BIN_DIR=$(INSTALL_DIR)/bin
INSTALL_SHARE_DIR=$(INSTALL_DIR)/share
INFNPATCH_DIR=$(ROOT_DIR)/mtb_shared/infn-patch
M0_DIR=$(ROOT_DIR)/src/app_cm0p
M0_TARGET_DIR=$(M0_DIR)/libs/TARGET_CY8CKIT-062S2-43012
M4_DIR=$(ROOT_DIR)/src/app_cm4
M4_TARGET_DIR=$(M4_DIR)/libs/TARGET_CY8CKIT-062S2-43012
GENTOC2_DIR=$(ROOT_DIR)/gentoc2
RSABIN_DIR=$(ROOT_DIR)/rsakeys
AMCU_PRIVATE_KEY=$(RSABIN_DIR)/amcu_private_key.pem

ifeq ($(findstring /cygdrive/c,$(ROOT_DIR)),/cygdrive/c)
ROOT_DIR:=$(subst /cygdrive/c,C:,$(ROOT_DIR))
endif

# Standard utilities
CAT      := cat
CD       := cd
CP       := cp -r
MV       := mv
CMAKE    := cmake
ECHO 	 := echo
MKDIR    := mkdir -p
OBJCOPY  := objcopy
RM       := rm -rf
SPLIT    := split
SHA256SUM := sha256sum
TRUNCATE := truncate
OPENSSL  := openssl pkeyutl -sign
SREC_CAT := srec_cat
SED := sed
VER_NUMS='0.0.0.0' # GGG.MMM.mmm.PPP
#Metadata register in hex is 0x9050_0000
#Silicon ID value at 0x9050_0000[2]=0xE4531202
#Keep in mind that checksum bytes at the end will change if the values are modified
JTAGIDFORHEX=$(shell printf '"1s/^/%s%s/"' ":0200000490501A\n" ":060000000000E4531202AF\n")

#
# File manipulation constants
#
#Start of App-A, should match include/amcu_mem_defs.h
AMCUIMAGEA_OFF = 0x10000000
# IMAGE Size = Application + VERNUM+SHA256SUM+DIGSIG+CALCMN+CALBBRD
AMCUIMAGE_SIZE = 1M
#Start of App-B, should match include/amcu_mem_defs.h
AMCUIMAGEB_OFF = 0x10100000
# APPLICATION+FOOTER+CALCMN
AMCUAPP_CALCMN_SIZE = 16640
# AMCUAPP_END = AMCUAPP.BIN - CALCMSIZE
AMCUAPP_END  = 16384
# AMCUFOOTER_OFFSET = AMCUAPP_END - FOOTERSIZE (384)
AMCUAPP_FOOTER_OFFSET = 16000
CY_DIGSIG_SIZE = 256
VERNUM_SIZE = 64
SHA256SUM_SIZE = 64
#SFlash public key
AMCUPKEY_OFF = 0x16005A00
AMCU_PKEY_SIZE = 1096
#toc2 region
AMCUTOC2_OFF = 0x16007C00
AMCU_TOC2_SIZE = 512
#
# No default rules
#
MAKEFLAGS+=-r

export

DIRS=src
CLEANDIRS=$(DIRS)
CLEAN_ALL_DIRS=$(addprefix cleanall-,$(CLEANDIRS) include)

.PHONY: help all $(DIRS) TAGS $(CLEAN_ALL_DIRS) database db dbs cleandatabase cleandatabases cleandb dumpdb dumpdatabases dumpdbs dumpdatabase format gentoc2 amcuapp-build amcuapp

.PHONY: $(INFNPATCH_DIR)/cy8c6xxa_cm0plusA.ld $(INFNPATCH_DIR)/cy8c6xxa_cm0plusB.ld $(INFNPATCH_DIR)/cy8c6xxa_cm4_dualA.ld $(INFNPATCH_DIR)/cy8c6xxa_cm4_dualB.ld

all: $(DIRS)

$(INFNPATCH_DIR)/cy8c6xxa_cm0plusA.ld:
	$(ROOT_DIR)/../tools/build/gen-lds.sh $(INFNPATCH_DIR)/cy8c6xxa_cm0plusA.ld.in $(ROOT_DIR)/include $(INFNPATCH_DIR)/cy8c6xxa_cm0plusA.ld /opt/ModusToolbox/tools_3.0/gcc/bin/arm-none-eabi-gcc

$(INFNPATCH_DIR)/cy8c6xxa_cm0plusB.ld:
	$(ROOT_DIR)/../tools/build/gen-lds.sh $(INFNPATCH_DIR)/cy8c6xxa_cm0plusB.ld.in $(ROOT_DIR)/include $(INFNPATCH_DIR)/cy8c6xxa_cm0plusB.ld /opt/ModusToolbox/tools_3.0/gcc/bin/arm-none-eabi-gcc

$(INFNPATCH_DIR)/cy8c6xxa_cm4_dualA.ld:
	$(ROOT_DIR)/../tools/build/gen-lds.sh $(INFNPATCH_DIR)/cy8c6xxa_cm4_dualA.ld.in $(ROOT_DIR)/include $(INFNPATCH_DIR)/cy8c6xxa_cm4_dualA.ld /opt/ModusToolbox/tools_3.0/gcc/bin/arm-none-eabi-gcc

$(INFNPATCH_DIR)/cy8c6xxa_cm4_dualB.ld:
	$(ROOT_DIR)/../tools/build/gen-lds.sh $(INFNPATCH_DIR)/cy8c6xxa_cm4_dualB.ld.in $(ROOT_DIR)/include $(INFNPATCH_DIR)/cy8c6xxa_cm4_dualB.ld /opt/ModusToolbox/tools_3.0/gcc/bin/arm-none-eabi-gcc

install: amcu-compile
	@echo "Installing project in $(INSTALL_DIR)"
	$(MKDIR) $(INSTALL_BIN_DIR)
	$(MKDIR) $(INSTALL_SHARE_DIR)
	$(CP) $(M4_DIR)/build/CY8CKIT-062S2-43012/Debug/amcuapp* $(INSTALL_SHARE_DIR)/.
	$(MV) $(INSTALL_SHARE_DIR)/amcuapp.hex $(INSTALL_BIN_DIR)/.
	$(CD) $(INSTALL_SHARE_DIR); \
	$(RM) *.c;

amcu-compile:
	@echo "...Compiling AMCU image"
	$(MAKE) --directory=$(M4_DIR) build;

amcuapp-build: $(INFNPATCH_DIR)/cy8c6xxa_cm0plusA.ld $(INFNPATCH_DIR)/cy8c6xxa_cm0plusB.ld $(INFNPATCH_DIR)/cy8c6xxa_cm4_dualA.ld $(INFNPATCH_DIR)/cy8c6xxa_cm4_dualB.ld
	@echo "Running amcuapp build for images A and B"
	export CY_TOOLS_PATHS=/opt/ModusToolbox/tools_3.0/; \
	$(RM) $(DEPLOY_DIR); \
	$(MKDIR) $(DEPLOY_DIR); \
	$(MAKE) --directory=$(ROOT_DIR)/src; \
	$(CP) $(INFNPATCH_DIR)/cy8c6xxa_cm0plusA.ld $(M0_TARGET_DIR)/COMPONENT_CM0P/TOOLCHAIN_GCC_ARM/cy8c6xxa_cm0plus.ld; \
	$(CP) $(INFNPATCH_DIR)/cy8c6xxa_cm4_dualA.ld $(M4_TARGET_DIR)/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/cy8c6xxa_cm4_dual.ld; \
	$(MAKE) --directory=$(ROOT_DIR) install; \
	$(MV) $(INSTALL_BIN_DIR)/amcuapp.hex $(DEPLOY_DIR)/amcuappA.hex; \
	$(CP) $(INFNPATCH_DIR)/cy8c6xxa_cm0plusB.ld $(M0_TARGET_DIR)/COMPONENT_CM0P/TOOLCHAIN_GCC_ARM/cy8c6xxa_cm0plus.ld; \
	$(CP) $(INFNPATCH_DIR)/cy8c6xxa_cm4_dualB.ld $(M4_TARGET_DIR)/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/cy8c6xxa_cm4_dual.ld; \
	$(MAKE) --directory=$(ROOT_DIR) install; \
	$(MV) $(INSTALL_BIN_DIR)/amcuapp.hex $(DEPLOY_DIR)/amcuappB.hex;

amcuapp: amcuapp-build
	@echo "Restructuring images, output to $(DEPLOY_DIR)"
	$(CD) $(DEPLOY_DIR); \
	$(OBJCOPY) -I ihex -O binary -R .sec12 amcuappA.hex amcuappA.bin; \
	$(TRUNCATE) -s $(AMCUAPP_CALCMN_SIZE) amcuappA.bin; \
	$(SPLIT) -a 1 -d -b $(AMCUAPP_END) amcuappA.bin amcuappA.bin_; \
	$(TRUNCATE) -s $(AMCUAPP_FOOTER_OFFSET) amcuappA.bin_0; \
	$(ECHO) $(VER_NUMS) > version_nums.txt; \
	$(TRUNCATE) -s $(VERNUM_SIZE) version_nums.txt; \
	$(CAT) amcuappA.bin_0 version_nums.txt > amcuappA.bin_ver_0; \
	$(SHA256SUM) amcuappA.bin_ver_0 > amcuappA.sha256; \
	$(TRUNCATE) -s $(SHA256SUM_SIZE) amcuappA.sha256; \
	$(CAT) amcuappA.bin_ver_0 amcuappA.sha256 > amcuappA.bin_ver_sha_0; \
	$(OPENSSL) -in amcuappA.bin_ver_sha_0 -rawin -out amcuappA_digisign.bin -inkey $(AMCU_PRIVATE_KEY); \
	$(CAT) amcuappA.bin_ver_sha_0  amcuappA_digisign.bin amcuappA.bin_1 > amcuappA.bin; \
	$(OBJCOPY) -I ihex -O binary -R .sec12 amcuappB.hex amcuappB.bin; \
	$(TRUNCATE) -s $(AMCUAPP_CALCMN_SIZE) amcuappB.bin; \
	$(SPLIT) -a 1 -d -b $(AMCUAPP_END) amcuappB.bin amcuappB.bin_; \
	$(TRUNCATE) -s $(AMCUAPP_FOOTER_OFFSET) amcuappB.bin_0; \
	$(CAT) amcuappB.bin_0 version_nums.txt > amcuappB.bin_ver_0; \
	$(SHA256SUM) amcuappB.bin_ver_0 > amcuappB.sha256; \
	$(TRUNCATE) -s $(SHA256SUM_SIZE) amcuappB.sha256; \
	$(CAT) amcuappB.bin_ver_0 amcuappB.sha256 > amcuappB.bin_ver_sha_0; \
	$(OPENSSL) -in amcuappB.bin_ver_sha_0 -rawin -out amcuappB_digisign.bin -inkey $(AMCU_PRIVATE_KEY); \
	$(CAT) amcuappB.bin_ver_sha_0  amcuappB_digisign.bin amcuappB.bin_1 > amcuappB.bin; \

gentoc2:
	@echo "Running gentoc2 (linux) build"
	$(CD) $(GENTOC2_DIR); \
	$(RM) build bin; \
	$(MKDIR) build; \
	$(CMAKE) -DCMAKE_TOOLCHAIN_FILE=$(ROOT_DIR)/../x86-linux_toolchain.cmake -B build; \
	$(MAKE) install -C build; \
	$(CD) $(RSABIN_DIR); \
	$(RM) *.bin; \
	$(RM) *.hex; \
	$(GENTOC2_DIR)/bin/gentoc2; \
	$(TRUNCATE) -s $(AMCU_PKEY_SIZE) amcu_public_key_zero.bin; \
	$(TRUNCATE) -s $(AMCU_TOC2_SIZE) amcu_toc2_zero.bin
	$(SREC_CAT) $(RSABIN_DIR)/amcu_public_key_zero.bin -binary -offset $(AMCUPKEY_OFF) -o $(RSABIN_DIR)/amcu_public_key_zero.hex -intel; \
	$(SREC_CAT) $(RSABIN_DIR)/amcu_public_key.bin -binary -offset $(AMCUPKEY_OFF) -o $(RSABIN_DIR)/amcu_public_key.hex -intel; \
	$(SREC_CAT) $(RSABIN_DIR)/amcu_toc2_zero.bin -binary -offset $(AMCUTOC2_OFF) -o $(RSABIN_DIR)/amcu_toc2_zero.hex -intel; \
	$(SREC_CAT) $(RSABIN_DIR)/amcu_toc2.bin -binary -offset $(AMCUTOC2_OFF) -o $(RSABIN_DIR)/amcu_toc2.hex -intel; \

$(DIRS):
	@echo "Entering $@"
	$(Q)$(MAKE) -C $@

cleanall: $(CLEAN_ALL_DIRS)
	$(Q)-$(RM) *~
	$(RM) $(INSTALL_DIR)
	@echo "----| Removing $(INSTALL_DIR) dir"

$(CLEAN_ALL_DIRS):
	@echo "----| Cleaning up $(patsubst cleanall-%,%,$@)"
	$(Q)$(MAKE) -C $(patsubst cleanall-%,%,$@) cleanalldir
