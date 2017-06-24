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
#    [default]    : Build target executable
#    build        : Build target executable
#    compile-all  : Compile all sources into object files
#    clean        : Remove all generated files
#    <file>.o     : Compile a single .c/.S source file
#    <file>.i     : Precompile a single source file
#    <file>.d     : Create the dependency file for a source file
#    <file>.asm   : Compile a single C source file into assembly

ifndef PLATFORM
  $(info PLATFORM not defined, assuming PLATFORM=HOST)
  PLATFORM=HOST
endif


### FLAGS ###

# The name of our primary build target, minus extension
TARGET=project1

# Compiler, preprocessor, and linker flags common to all platforms
CFLAGS = -Wall -Werror -g -O0 -std=c99
INCLUDES = -I../include/common
CPPFLAGS = -DPROJECT1 $(INCLUDES)
CPPFLAGS += -DVERBOSE
LDFLAGS = -Wl,-Map=$(TARGET).map

# Platform-specific flags and compliler selection
ifeq ($(PLATFORM),HOST)
#  CPPFLAGS += -DNO_STDIO
  CC = gcc
else ifeq ($(PLATFORM),BBB)
  CC = arm-linux-gnueabihf-gcc
else ifeq ($(PLATFORM),KL25Z)
  CC = arm-none-eabi-gcc
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
  INCLUDES += -I../include/CMSIS -I../include/kl25z
  LDFLAGS += -T../platform/MKL25Z128xxx4_flash.ld
  LDFLAGS += -specs=nosys.specs
else
  $(error Invalid PLATFORM specified, must be one of: HOST, BBB, KL25Z)
endif


### FILES ###

include sources.mk

# TODO: separate build dirs for each platform
OBJECTS = $(SOURCES:.c=.o) $(ASM_SOURCES:.S=.o)
DEPENDENCIES = $(OBJECTS:.o=.d)


### RULES ###

# default rule
.PHONY: build
build: $(TARGET).elf $(DEPENDENCIES)

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

# Compile to object files, no linking
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
%.o: %.S
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Link/locate the project executable
$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.PHONY: clean
clean:
	rm -f *.o *.i *.elf *.map *.d *.asm

