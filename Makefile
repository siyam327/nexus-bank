CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -Iinclude -lm
SRC_DIR = src
BUILD   = build
TARGET  = bank

# Grab every .c file in src/
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD)/%.o)

.PHONY: all clean run

all: $(BUILD) data $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -lm
	@echo ""
	@echo "  Build successful! Run with:  ./$(TARGET)"
	@echo ""

$(BUILD)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

data:
	mkdir -p data

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD) $(TARGET)
	@echo "Cleaned."

# Windows users with MinGW: run  mingw32-make  instead of  make
