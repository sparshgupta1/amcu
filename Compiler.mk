# Compiler.mk
#
# Provide compiler abstraction via the defined architecture, TGT_ARCH.
# TGT_ARCH can be one of 'PetaLinux', 'armr5', 'aarch64'.
#
# The flags here are system-wide flags. Be hesitant to changing them.
# If a sub-project needs some flags, you can add to the flags defined
# here, in that sub-project's Makefile.

# Those are the architectures we support.
# Those definitions are used by Compiler.mk.
#
PETALINUX=PetaLinux
ARMR5=armr5
AARCH64=aarch64

ARCH_DEFAULT=$(AARCH64)

ifeq ($(MAKECMDGOALS),clean)
else ifeq ($(MAKECMDGOALS),help)
else ifeq ($(TGT_ARCH),)
TGT_ARCH:=$(ARCH_DEFAULT)
$(warning You didn't specify the architecture for me to use, so I'm using "$(TGT_ARCH)".)
export TGT_ARCH
endif

ifeq ($(shell uname -s),Linux)
OS_PREFIX=lin
else
OS_PREFIX=nt
endif

# All newer versions of GCC past 4.9.2 use the GNU11 standard.
# Set it here explicity, as we don't want to be surpirsed.
#
C_STANDARD=-std=gnu11
CPP_STANDARD=-fpermissive --std=c++11

# Determine if dbg symbols should be used
ifeq ($(DBG),)
	CPPFLAGS+=-s
	CFLAGS+=-s
else
    CPPFLAGS+=-g
    CFLAGS+=-g
    DBG_DEF := True
endif


ifeq ($(TGT_ARCH),$(PETALINUX))
#
# Tool chain for building applications to run under PetaLinux,
# on an ARM Cortex-A53, ARMv8-A architecture (aarch64).
#
TARGET_TOOL_PATH=/opt/xilinx-sdk-2019.1
TOOLS_DIR=$(TARGET_TOOL_PATH)/sysroots/x86_64-petalinux-linux/usr
ABI_STR=aarch64-xilinx-linux
ABI_NUM=8.2.0


BIN_DIR=$(TOOLS_DIR)/bin/aarch64-xilinx-linux
BIN_PREFIX=$(ABI_STR)-

CC=$(BIN_DIR)/$(BIN_PREFIX)gcc --sysroot=$(TARGET_TOOL_PATH)/sysroots/aarch64-xilinx-linux
CPP=$(BIN_DIR)/$(BIN_PREFIX)cpp 
AS=$(BIN_DIR)/$(BIN_PREFIX)as
AR=$(BIN_DIR)/$(BIN_PREFIX)ar
RANLIB=$(BIN_DIR)/$(BIN_PREFIX)ranlib
CXX=$(BIN_DIR)/$(BIN_PREFIX)g++ --sysroot=$(TARGET_TOOL_PATH)/sysroots/aarch64-xilinx-linux
OBJDUMP=$(BIN_DIR)/$(BIN_PREFIX)objdump 
LD=$(BIN_DIR)/$(BIN_PREFIX)ld --sysroot=$(TARGET_TOOL_PATH)/sysroots/aarch64-xilinx-linux

# Location of cc1.
PATH:=$(TOOLS_DIR)/libexec/gcc/$(ABI_STR)/$(ABI_NUM):$(BIN_DIR):$(PATH)

INCLUDE_DIR+=-I$(ROOT_DIR)/include \
	-I$(ROOT_DIR)/../fpgahw \
	-I$(ROOT_DIR)/src/mc \
	-I$(ROOT_DIR)/../fpgalib \
	-I$(ROOT_DIR)/../include

CPPFLAGS+=$(INCLUDE_DIR)
CFLAGS+=$(C_STANDARD) -Wall -Wextra -Werror
LDFLAGS+= -lm -lrt -lpthread -lsystemd -levent_core

else ifeq ($(TGT_ARCH),$(AARCH64))
#
# Tool chain for building applications to run on an ARM Cortex-A53,
# ARMv8-A architecture (aarch64). This is baremetal "none" use
#

TOOLS_DIR=/opt/arm/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf
ABI_STR=aarch64-none-elf
ABI_NUM=10.3.1


BIN_DIR=$(TOOLS_DIR)/bin/
BIN_PREFIX=$(ABI_STR)-

CC=$(BIN_DIR)/$(BIN_PREFIX)gcc
CPP=$(BIN_DIR)/$(BIN_PREFIX)cpp
AS=$(BIN_DIR)/$(BIN_PREFIX)as
AR=$(BIN_DIR)/$(BIN_PREFIX)ar
RANLIB=$(BIN_DIR)/$(BIN_PREFIX)ranlib
CXX=$(BIN_DIR)/$(BIN_PREFIX)g++
OBJDUMP=$(BIN_DIR)/$(BIN_PREFIX)objdump
LD=$(BIN_DIR)/$(BIN_PREFIX)ld

# Location of cc1.
PATH:=$(TOOLS_DIR)/libexec/gcc/$(ABI_STR)/$(ABI_NUM):$(BIN_DIR):$(PATH)

INCLUDE_DIR+=-I$(ROOT_DIR)/include \
	-I$(ROOT_DIR)/../fpgahw \
	-I$(ROOT_DIR)/src/mc \
	-I$(ROOT_DIR)/../fpgalib \
	-I$(ROOT_DIR)/../include

CPPFLAGS+=$(INCLUDE_DIR)  -Wno-error
CFLAGS+= -ffreestanding -mcpu=cortex-a53 -Os -DSQLITE_OMIT_LOAD_EXTENSION $(C_STANDARD) -Werror -Wno-pointer-to-int-cast -ffunction-sections -fdata-sections -DWAA400_UART_ENABLED -DLOG_INTERNAL_FIFO -DLOG_FIFO_PLATFORM=2

CXXFLAGS+=$(CPP_STANDARD) $(INCLUDE_DIR) -Os -DSQLITE_OMIT_LOAD_EXTENSION  -Wno-error
LDFLAGS+= -Wl,--gc-sections

else ifeq ($(TGT_ARCH),$(ARMR5))
#
# Tool chain to build a bare metal app running on ARM R5,
# ARMv7-R architecture.
#
#TOOLS_DIR=$(SDK_COMPILER_DIR)/armr5/$(OS_PREFIX)/gcc-arm-none-eabi
TOOLS_DIR=/opt/Xilinx/SDK/2019.1/gnu/armr5/$(OS_PREFIX)/gcc-arm-none-eabi
ABI_STR=arm-none-eabi
ABI_NUM=8.2.0

BIN_DIR=$(TOOLS_DIR)/bin
BIN_PREFIX=armr5-none-eabi-

CC=$(BIN_DIR)/$(BIN_PREFIX)gcc --sysroot=$(TOOLS_DIR)
CPP=$(BIN_DIR)/$(BIN_PREFIX)cpp
AS=$(BIN_DIR)/$(BIN_PREFIX)as
AR=$(BIN_DIR)/$(BIN_PREFIX)ar
RANLIB=$(BIN_DIR)/$(BIN_PREFIX)ranlib
CXX=$(BIN_DIR)/$(BIN_PREFIX)g++ --sysroot=$(TOOLS_DIR)
OBJDUMP=$(BIN_DIR)/$(BIN_PREFIX)objdump
LD=$(BIN_DIR)/$(BIN_PREFIX)ld --sysroot=$(TOOLS_DIR)

# Location of cc1.
PATH:=$(TOOLS_DIR)/lib/gcc/$(ABI_STR)/$(ABI_NUM):$(BIN_DIR):$(PATH)

INCLUDE_DIR+=-I$(TOOLS_DIR)/$(ABI_STR)/include -I$(TOOLS_DIR)/lib/gcc/$(ABI_STR)/$(ABI_NUM)/include -I$(TOOLS_DIR)/lib/gcc/$(ABI_STR)/$(ABI_NUM)/include-fixed -I$(ROOT_DIR)/include -I$(ROOT_DIR)/../fpgahw

CPPFLAGS+=$(INCLUDE_DIR)
CFLAGS+=-ffreestanding -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -Os -fPIC $(C_STANDARD) -mfp16-format=ieee -ffunction-sections -fdata-sections -Wall -Wextra -Werror
CXXFLAGS+=$(INCLUDE_DIR) -ffreestanding -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -Os -fPIC -Wall -Wextra -Werror
LDFLAGS+= -Wl,--gc-sections

############### All else ###############
else ifeq ($(MAKECMDGOALS),clean)
else ifeq ($(MAKECMDGOALS),help)
else

$(error Sorry, I do not recognize "$(TGT_ARCH)" architecture. Try 'make help')


endif
