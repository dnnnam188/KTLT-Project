# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Source files
SRCS = main.cpp \
       $(SRC_DIR)/file_io.cpp \
       $(SRC_DIR)/gpa.cpp \
       $(SRC_DIR)/algorithms.cpp \
       $(SRC_DIR)/statistics.cpp \
       $(SRC_DIR)/string_utils.cpp \
       $(SRC_DIR)/utils.cpp

# Object files
OBJS = $(OBJ_DIR)/main.o \
       $(OBJ_DIR)/file_io.o \
       $(OBJ_DIR)/gpa.o \
       $(OBJ_DIR)/algorithms.o \
       $(OBJ_DIR)/statistics.o \
       $(OBJ_DIR)/string_utils.o \
       $(OBJ_DIR)/utils.o

# Output executable
TARGET = student_gpa

# Default target
all: $(OBJ_DIR) $(TARGET)

# Create obj directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile main.cpp
$(OBJ_DIR)/main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile src/*.cpp
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Run with test1
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
