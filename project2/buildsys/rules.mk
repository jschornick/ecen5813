### rules.mk ###

# Default rule
# Build target executable from object files and report size
.PHONY: build
build: $(EXE)
	@echo "\nBuild report:"
	@$(SIZE) $<

# Compile all object files, no linking
.PHONY: compile-all
compile-all: $(OBJECTS)

# Shorthand targets, where the build directory is ommitted
%.o: $(BUILD_DIR)/%.o ;
%.i: $(BUILD_DIR)/%.i ;
%.asm: $(BUILD_DIR)/%.asm ;
# Don't delete prerequisites that should be kept
.PRECIOUS: $(BUILD_DIR)/%.i $(BUILD_DIR)/%.asm

# Preprocess only
$(BUILD_DIR)/%.i: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) -E $< -o $@
$(BUILD_DIR)/%.i: $(SRC_DIR)/%.S | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) -E $< -o $@

# Compile but leave as assembly
$(BUILD_DIR)/%.asm: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -S $< -o $@

# Compile C or assembly to object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Link/locate the project executable directly with object files
$(EXE): $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# clean selected platforms
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(EXE)

# clean all all platforms
.PHONY: clean-all
clean-all:
	rm -rf $(BUILD_BASE) $(dir $(EXE))*.elf
