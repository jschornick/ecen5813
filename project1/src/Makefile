# Makefile for Project 1
#
# Supports building for three different platforms:
#    HOST  : A linux host system, using system system default GCC
#    BBB   : Beaglebone Black, running Linux
#    KL25Z : NXP/Freescale KL25Z, bare metal
#
# Platform selection defaults to HOST, but may be overridden on the command line:
#    $ make [target(s)] PLATFORM={HOST,BBB,KL25Z}
#
# Supported targets include:
#    [default]     : Build target executable
#    build         : Build target executable
#    build-lib     : Build target executable from library archive
#    build-ld      : Build target by directly invoking the linker
#    compile-all   : Compile all sources into object files
#    clean         : Remove all generated files
#    <file>.o      : Compile a single .c/.S source file
#    <file>.i      : Precompile a single source file
#    <file>.d      : Create the dependency file for a source file
#    <file>.asm    : Compile a single C source file into assembly
#    libproject1.a : Create library of platform independent functions

ifndef PLATFORM
  $(info PLATFORM not defined, assuming PLATFORM=HOST)
  PLATFORM=HOST
endif


### FLAGS ###

# The name of our primary build target, minus extension
TARGET = project1
INCDIR = ../include
LIBNAME = lib$(TARGET).a

# Compiler, preprocessor, and linker flags common to all platforms
CFLAGS = -std=c99
CFLAGS += -Wall -Werror
CFLAGS += -g -O0

CPPFLAGS = -DPROJECT1
CPPFLAGS += $(INCFLAGS)
CPPFLAGS += -DVERBOSE
INCFLAGS = -I$(INCDIR)/common

LDFLAGS = -Wl,-Map=$(TARGET).map
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
  # Note: The FPU-realted CFLAGS from he project document were removed as they
  #       only apply to the Cortex-M4
  #       See: /usr/share/doc/gcc-arm-none-eabi/readme.txt.gz
  INCFLAGS += -I$(INCDIR)/CMSIS
  INCFLAGS += -I$(INCDIR)/kl25z
  LDFLAGS += -T../platform/MKL25Z128xxx4_flash.ld
  LDFLAGS += -specs=nosys.specs
else
  $(error Invalid PLATFORM specified, must be one of: HOST, BBB, KL25Z)
endif

# Flags for manually calling the linker
include link_flags.mk


### TOOLCHAIN / BINUTILS ###

CC = $(TOOLCHAIN)gcc
LD = $(TOOLCHAIN)ld
SIZE = $(TOOLCHAIN)size
AR = $(TOOLCHAIN)ar


### FILES ###

include sources.mk

# A list of all platform independent object files
PI_OBJS = $(SOURCES:.c=.o)
# A list of all platform dependent object files
PD_OBJS = $(PLATFORM_SRCS:.c=.o) $(ASM_SRCS:.S=.o)
# A list of all object files needed to build target
# (excluding system library dependencies)
OBJECTS = $(PI_OBJS) $(PD_OBJS)

DEPENDENCIES = $(OBJECTS:.o=.d)


### RULES ###

# Default rule
# Build target executable from object files and report size
.PHONY: build
build: $(TARGET).elf $(DEPENDENCIES)
	@echo "\nBuild report:"
	@$(SIZE) $<

# Build target executable using library archive
.PHONY: build-lib
build-lib: $(TARGET)_lib.elf

# Link target executable directly via LD (no GCC)
.PHONY: build-ld
build-ld: $(TARGET)_ld.elf

# Compile all object files, no linking
.PHONY: compile-all
compile-all: $(OBJECTS)

# Preprocess only
%.i: %.c
	$(CC) $(CPPFLAGS) -E $< -o $@

# Calculated dependencies
%.d: %.c
	$(CC) $(CPPFLAGS) -M $< -o $@
%.d: %.S
	$(CC) $(CPPFLAGS) -M $< -o $@

# Compile but leave as assembly
%.asm: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -S $< -o $@

# Compile C or assembly to object file
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
%.o: %.S
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Build library of platform-independent object files
$(LIBNAME): $(PI_OBJS)
	$(AR) rv $(LIBNAME) $(PI_OBJS)

# Link/locate the project executable directly with object files
$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# Link/locate the project executable from library
$(TARGET)_lib.elf: $(LIBNAME) $(PD_OBJS)
	$(CC) $(LDFLAGS) $(LIBNAME) $(PD_OBJS) -o $@

# Link/locate the project executable by calling LD directly (no GCC)
# NOTE: Order matters -- LD_OBJS_PRE must come before LD_LIBS and LD_OBJS, OBJECTS before LD_LIBS
$(TARGET)_ld.elf: $(OBJECTS)
	$(LD) $(LD_OPTS) $(LD_PATH) $(LD_OBJS_PRE) $(OBJECTS) $(LD_LIBS) $(LD_OBJS) -o $@

.PHONY: clean
clean:
	rm -f *.o *.i *.elf *.map *.d *.asm *.a

