# Makefile for Proect 2 tests

THIRD_PARTY=../../3rd-party
CMOCKA_LIB=$(THIRD_PARTY)/cmocka/BUILD/src/libcmocka.a
CMOCKA_INCLUDE+=-I$(THIRD_PARTY)/cmocka/include

PROJECT_INCLUDE=-I../include/common
PROJECT_LIB=../BUILDOUT/HOST/libproject2.a

CFLAGS=-Wall -Werror

TESTS=$(patsubst %.c, %.run, $(wildcard test_*.c))

runall: $(TESTS)
	@echo
	-@for x in $^; do echo $$x; ./$$x; echo; done

test_%.run: test_%.c $(CMOCKA_LIB) $(PROJECT_LIB)
	gcc $(CFLAGS) $(CMOCKA_INCLUDE) $(PROJECT_INCLUDE) $(PROJECT_LIB) $^ -o $@

$(CMOCKA_LIB):
	cd $(THIRD_PARTY); make cmocka

.PHONY: clean
clean:
	rm -rf *.run
