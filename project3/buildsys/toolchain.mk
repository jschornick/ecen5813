# toolchain.mk
#
# Toolchain selection and configuration

# Compiler, preprocessor, and linker flags common to all platforms
CFLAGS = -std=c99
CFLAGS += -Wall -Werror
CFLAGS += -g -O0

CPPFLAGS += $(INCFLAGS)
INCFLAGS = -I$(INC_DIR)/common

CPPFLAGS += $(PROJFLAGS)

MAPFILE=$(BUILD_DIR)/$(TARGET).map
LDFLAGS = -Wl,-Map=$(MAPFILE)
#LDFLAGS = -v  # verbose output during linking

# Platform-specific flags
ifeq ($(PLATFORM),HOST)
  TOOLCHAIN =
  #CPPFLAGS += -DNO_STDIO
else ifeq ($(PLATFORM),BBB)
  TOOLCHAIN = arm-linux-gnueabihf-
else ifeq ($(PLATFORM),KL25Z)
  TOOLCHAIN = arm-none-eabi-
  # Disable standard I/O functions (printf, etc), enable HW peripherals
  CPPFLAGS += -DKL25Z
  ifdef KL25Z_USE_DMA
    CPPFLAGS += -DMEMORY_USES_DMA
  endif
  # Compile for 48MHz core, 24MHz bus operation (PEE mode)
  CPPFLAGS += -DCLOCK_SETUP=1
  # Use of 'march' _should_ be redundant here, as armv6-m will be implied by
  # specifying 'mcpu', but the resulting binaries are not the same
  CFLAGS += -mcpu=cortex-m0plus
  CFLAGS += -march=armv6-m
  # Generate code for Thumb mode, required for Cortex-M
  CFLAGS += -mthumb
  INCFLAGS += -I$(INC_DIR)/CMSIS
  INCFLAGS += -I$(INC_DIR)/kl25z
  LDFLAGS += -T$(PLAT_DIR)/MKL25Z128xxx4_flash.ld
  LDFLAGS += -specs=nosys.specs
  # Both -march and -mthumb are required during linking via GCC, since it uses this information
  # to choose the appropriate C runtime variant (armv6-m).
  LDFLAGS += -march=armv6-m
  LDFLAGS += -mthumb
  FLASH_SCRIPT = $(PLAT_DIR)/openocd_kl25z_flash.cfg
else
  $(error Invalid PLATFORM specified, must be one of: HOST, BBB, KL25Z)
endif

# Define the compiler and binutils for our toolchain
CC = $(TOOLCHAIN)gcc
LD = $(TOOLCHAIN)ld
SIZE = $(TOOLCHAIN)size
AR = $(TOOLCHAIN)ar

OCD = openocd
