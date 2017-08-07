# Top-level Makefile for Project 3
#
# Supports building for three different platforms:
#    HOST  : A linux host system, using system system default GCC
#    BBB   : Beaglebone Black, running Linux
#    KL25Z : NXP/Freescale KL25Z, bare metal
#
# Platform selection defaults to HOST, but may be overridden on the command line:
#    $ make [target(s)] PLATFORM={HOST,BBB,KL25Z}
#
# Supported targets include (see rules.mk):
#    [default]     : Build target executable
#    build         : Build target executable
#    compile-all   : Compile all sources into object files
#    clean         : Remove all generated files
#    <file>.o      : Compile a single .c/.S source file
#    <file>.i      : Precompile a single source file
#    <file>.asm    : Compile a single C source file into assembly
#
# Additional files for the build system can be found under "buildsys".

ifndef PLATFORM
  $(info PLATFORM not defined, assuming PLATFORM=HOST)
  PLATFORM=HOST
endif

# The name of our primary build target, minus extension
TARGET = project3

# Makefile includes
BSYS_DIR = buildsys
# C and ASM source files
SRC_DIR = src
# C header files for preprocessor
INC_DIR = include
# Non-source platform configuration
PLAT_DIR = platform
# Top-level directory for build output
BUILD_BASE = BUILDOUT
# platform-specific build output
BUILD_DIR = $(BUILD_BASE)/$(PLATFORM)

# Filename for the built executable
EXE = $(TARGET)_$(PLATFORM).elf

# Project flags, interpretted by the preprocessor
PROJFLAGS = -DPROJECT3

## Logging Demo  ##
# Select logging enable/disable and format
#PROJFLAGS += -DDISABLE_LOG
#PROJFLAGS += -DLOG_OUT_BINARY
PROJFLAGS += -DLOG_OUT_ASCII
#PROJFLAGS += -DLOG_OUT_NULL

## Profiling  ##
# PROJFLAGS += -DPROFILER
# PROJFLAGS += -DDISABLE_LOG
# KL25Z_USE_DMA= 1
PROF_BUFFER_SIZE=5000
PROJFLAGS += -DPROF_BUFFER_SIZE=$(PROF_BUFFER_SIZE)

## NRF Demo ##
PROJFLAGS += -DNRF

## Data Processor
# Use DMA on the KL25Z
#PROJFLAGS += -DPROCESSOR
#KL25Z_UART_NONBLOCK=1

# Makefile includes for the build system
include $(BSYS_DIR)/toolchain.mk
include $(BSYS_DIR)/sources.mk
include $(BSYS_DIR)/rules.mk
