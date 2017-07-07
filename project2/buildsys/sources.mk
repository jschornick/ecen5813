# sources.mk
#
# Makefile include that explicitly defines our source files and respective
# object files.

# All source filenames are relative to the source directory.

# Common source files, all platforms
COMMON_SRCS = \
  conversion.c \
  main.c \
  memory.c \
  project2.c

# Platform-specific source files
ifeq ($(PLATFORM),HOST)
  # no additional sources
else ifeq ($(PLATFORM),BBB)
  # no additional sources
else ifeq ($(PLATFORM),KL25Z)
  PLATFORM_SRCS += system_MKL25Z4.c
  ASM_SRCS += startup_MKL25Z4.S
else
  $(error Invalid PLATFORM specified, must be one of: HOST, BBB, KL25Z)
endif

SOURCES=$(COMMON_SRCS) $(PLATFORM_SRCS) $(ASM_SRCS)

# A list of all platform independent object files
PI_OBJS = $(COMMON_SRCS:.c=.o)
# A list of all platform dependent object files
PD_OBJS = $(PLATFORM_SRCS:.c=.o) $(ASM_SRCS:.S=.o)
# A list of all object files needed to build target
# (excluding system library dependencies)
OBJECTS = $(PI_OBJS) $(PD_OBJS)

OBJECTS := $(addprefix $(BUILD_DIR)/, $(PI_OBJS) $(PD_OBJS))
SOURCES := $(addprefix $(SRC_DIR)/, $(SOURCES))
