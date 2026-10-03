CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

BUILD = build
BIN = bin
LIB = $(BUILD)/libcryptopals.a

# Challenge implementations
CH_SRC = $(wildcard set*/ch??.c)
CH_OBJ = $(patsubst %.c,$(BUILD)/%.o,$(CH_SRC))
CH_DEP = $(CH_OBJ:.o=.d)

# Challenge main files
MAIN_SRC = $(wildcard set*/ch*_main.c)

.PHONY: all clean

# ============================================================
# Executable names
# ============================================================

# set1/ch01_main.c -> bin/set1-ch01
TARGETS = $(foreach main,$(MAIN_SRC),$(BIN)/$(subst _main,,$(subst /,-,$(basename $(main)))))


all: $(LIB) $(TARGETS)


# ============================================================
# Static library
# ============================================================

$(LIB): $(CH_OBJ)
	@mkdir -p $(dir $@)
	ar rcs $@ $^


# ============================================================
# Object files + automatic header dependencies
# ============================================================

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@


# Include automatically generated .d files
-include $(CH_DEP)


# ============================================================
# Executables
# ============================================================

.SECONDEXPANSION:

# bin/set1-ch01 -> set1/ch01_main.c
$(BIN)/%: $$(subst -,/,$$*)_main.c $(LIB)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -L$(BUILD) -lcryptopals -lm -lssl -lcrypto -o $@


# ============================================================
# Cleanup
# ============================================================

clean:
	rm -rf $(BUILD) $(BIN)
