# sources.mk
#
# Makefile include that explicitly defines our source files and respective
# object files.

# All source filenames are relative to the source directory.

# Common source files, all platforms
COMMON_SRCS = \
  circular_buffer.c \
  conversion.c \
  io.c \
  main.c \
  memory.c \
  platform.c \
  processor.c \
  project2.c

# Platform-specific source files
ifeq ($(PLATFORM),HOST)
  # no additional sources
else ifeq ($(PLATFORM),BBB)
  # no additional sources
else ifeq ($(PLATFORM),KL25Z)
  PLATFORM_SRCS += system_MKL25Z4.c
  PLATFORM_SRCS += led.c uart.c
  ASM_SRCS += startup_MKL25Z4.S
else
  $(error Invalid PLATFORM specified, must be one of: HOST, BBB, KL25Z)
endif

SOURCES=$(COMMON_SRCS) $(PLATFORM_SRCS) $(ASM_SRCS)

# A list of all platform independent object files
PI_OBJS = $(addprefix $(BUILD_DIR)/, $(COMMON_SRCS:.c=.o))
# A list of all platform dependent object files
PD_OBJS = $(addprefix $(BUILD_DIR)/, $(PLATFORM_SRCS:.c=.o) $(ASM_SRCS:.S=.o))
# A list of all object files needed to build target
# (excluding system library dependencies)
OBJECTS = $(PI_OBJS) $(PD_OBJS)
SOURCES := $(addprefix $(SRC_DIR)/, $(SOURCES))

# The list of objects which should be included in the library
LIB_OBJS = $(filter-out $(BUILD_DIR)/main.o, $(PI_OBJS))
LIBNAME = $(BUILD_DIR)/lib$(TARGET).a
