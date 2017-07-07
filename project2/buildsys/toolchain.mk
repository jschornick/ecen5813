# toolchain.mk
#
# Toolchain selection and configuration

# Compiler, preprocessor, and linker flags common to all platforms
CFLAGS = -std=c99
CFLAGS += -Wall -Werror
CFLAGS += -g -O0

CPPFLAGS += $(INCFLAGS)
INCFLAGS = -I$(INC_DIR)/common

CFLAGS += $(PROJFLAGS)

LDFLAGS = -Wl,-Map=$(BUILD_DIR)/$(TARGET).map
#LDFLAGS = -v  # verbose output during linking

# Platform-specific flags
ifeq ($(PLATFORM),HOST)
  TOOLCHAIN =
  #CPPFLAGS += -DNO_STDIO
else ifeq ($(PLATFORM),BBB)
  TOOLCHAIN = arm-linux-gnueabihf-
else ifeq ($(PLATFORM),KL25Z)
  TOOLCHAIN = arm-none-eabi-
  # Disable standard library I/O functions (printf, etc)
  CPPFLAGS += -DNO_STDIO
  # Use of 'march' is most likely redundant here, as armv6-m will be implied by
  # specifying 'mcpu'
  CFLAGS += -mcpu=cortex-m0plus
  CFLAGS += -march=armv6-m
  # Generate code that executes in Thumb state
  CFLAGS += -mthumb
  INCFLAGS += -I$(INC_DIR)/CMSIS
  INCFLAGS += -I$(INC_DIR)/kl25z
  LDFLAGS += -T$(PLAT_DIR)/MKL25Z128xxx4_flash.ld
  LDFLAGS += -specs=nosys.specs
else
  $(error Invalid PLATFORM specified, must be one of: HOST, BBB, KL25Z)
endif

# Define the compiler and binutils for our toolchain
CC = $(TOOLCHAIN)gcc
LD = $(TOOLCHAIN)ld
SIZE = $(TOOLCHAIN)size
AR = $(TOOLCHAIN)ar

