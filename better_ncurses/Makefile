# Makefile for compiling and linking the main.cpp file with ncurses, form, and menu libraries

# Compiler and compiler flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall
LIBS = -lncurses -lform -lmenu

# Directories
SRC_DIR = src
INC_DIR = include
BIN_DIR = bin

# Source files and executable name
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
EXECUTABLE = $(BIN_DIR)/main

# Default target
all: $(EXECUTABLE)

# Ensure BIN_DIR exists before compiling
$(shell mkdir -p $(BIN_DIR))

# Linking step
$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) $^ -o $@ $(LIBS)

# Clean target
clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean
