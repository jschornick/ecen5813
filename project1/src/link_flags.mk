# linker_flags.mk
#
# Makefile flags for manually calling the linker

ifeq ($(PLATFORM),HOST)
	LD_OPTS = -dynamic-linker /lib64/ld-linux-x86-64.so.2

	CRT_DIR = /usr/lib/x86_64-linux-gnu
	LD_OBJS_PRE += $(CRT_DIR)/Scrt1.o
	LD_OBJS_PRE += $(CRT_DIR)/crti.o
	LD_OBJS += $(CRT_DIR)/crtn.o

	LD_PATH += -L/lib/x86_64-linux-gnu  # libc
	LD_LIBS = -lc

else ifeq ($(PLATFORM),BBB)
	LD_OPTS = -dynamic-linker /lib/ld-linux-armhf.so.3

	CRT_DIR = /usr/arm-linux-gnueabihf/lib
	LD_OBJS_PRE += $(CRT_DIR)/crt1.o
	LD_OBJS_PRE += $(CRT_DIR)/crti.o
	LD_OBJS += $(CRT_DIR)/crtn.o

	LD_PATH += -L/usr/arm-linux-gnueabihf/lib # libc
	LD_PATH += -L/usr/lib/gcc-cross/arm-linux-gnueabihf/6 # libgcc
	LD_LIBS = -lc -lgcc

else ifeq ($(PLATFORM),KL25Z)
  LD_OPTS = -T../platform/MKL25Z128xxx4_flash.ld

	GCC_CRT_DIR = /usr/lib/gcc/arm-none-eabi/4.9.3
	LD_OBJS_PRE += $(GCC_CRT_DIR)/crti.o
	LD_OBJS_PRE += $(GCC_CRT_DIR)/crtbegin.o
	LD_OBJS_PRE += /usr/lib/arm-none-eabi/lib/crt0.o
	LD_OBJS += $(GCC_CRT_DIR)/crtend.o
	LD_OBJS += $(GCC_CRT_DIR)/crtn.o

	LD_PATH = -L/usr/lib/arm-none-eabi/newlib  # libc, libnosys
	LD_PATH += -L/usr/lib/gcc/arm-none-eabi/4.9.3  # libgcc
	LD_LIBS = -lgcc -lc -lnosys

endif
