# testing.mk
#
# Makefile include for testing targets

TEST_DIR = ../test
TESTS = test_convert

.PHONY: tests
tests: $(TESTS)

test_%: $(TEST_DIR)/test_%.c $(PI_OBJS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(filter-out main.o, $(PI_OBJS)) $< -o $@

.PHONY: runtests
runtests: tests
	$(foreach x, $(TESTS), ./$(x))

