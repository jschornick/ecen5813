# sources.mk
#
# Makefile include that explicitly defines our source files

# Common source files, all platforms
SOURCES = \
  conversion.c \
  debug.c \
  main.c \
  memory.c \
  project1.c

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

