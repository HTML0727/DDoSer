CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LIBS = -lncurses -lpthread -lcjson -lm

# Directories
SRC_DIR = src
INC_DIR = include
BIN_DIR = bin

# Source files
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/attack.c $(SRC_DIR)/network.c $(SRC_DIR)/ui.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = $(BIN_DIR)/ddos-tool

# Default target
all: directories $(TARGET)

# Create directories
directories:
	mkdir -p $(BIN_DIR)

# Build executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)

# Compile source files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y libncurses5-dev libcjson-dev

# Run the tool
run: $(TARGET)
	sudo ./$(TARGET)

.PHONY: all clean install-deps run directories