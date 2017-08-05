# Makefile for KL25Z platform-specific tests

ifndef PLATFORM
  PLATFORM=KL25Z
endif

# Relative location of main project
PRJ_DIR=../..

PLAT_DIR=$(PRJ_DIR)/platform
INC_DIR=$(PRJ_DIR)/include
include $(PRJ_DIR)/buildsys/toolchain.mk

# Unity configuration
UNITY_DIR=$(PRJ_DIR)/../3rd-party/unity
CPPFLAGS+=-DUNITY_INCLUDE_CONFIG_H
CPPFLAGS+=-DUNITY_VERBOSE
UNITY_INCLUDES=-I$(UNITY_DIR)/src -I$(UNITY_DIR)/extras/fixture/src

# Filenames specific to the test binary being built
TEST_TARGET=alltests_$(PLATFORM).elf
MAPFILE=alltests_$(PLATFORM).map
FLASH_SCRIPT=openocd_kl25z_flashtests.cfg

# Link with the library from the main project
PRJ_BUILD=$(PRJ_DIR)/BUILDOUT/$(PLATFORM)
PRJ_LIB=$(PRJ_BUILD)/libproject3.a

SRCS=\
  $(UNITY_DIR)/src/unity.c \
  $(UNITY_DIR)/extras/fixture/src/unity_fixture.c \
  test_memdma.c \
  test_runner.c \
  unity_helpers.c

INCLUDES=-I. $(INCFLAGS) $(UNITY_INCLUDES)

alltests:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(SRCS) $(PRJ_LIB) $(LDFLAGS) -o $(TEST_TARGET)

.PHONY: flashtests
ifeq ($(PLATFORM), KL25Z)
flashtests: alltests
	@echo
	@echo Flashing and reading results over serial device:
	@echo
	@$(OCD) -f $(FLASH_SCRIPT) --log_output /tmp/openocd_flash.log &
	@awk '1;/^OK|^FAIL/{exit}' /dev/ttyACM0
	@echo
else
flashtests:
	@echo Flashing only supported on KL25Z
endif

# Testing build system by running binary compiled for host
.PHONY: runtests
runtests: alltests
	@echo
	- ./$(TEST_TARGET) -v

clean:
	rm -rf *.elf *.map
