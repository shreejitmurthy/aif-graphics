# Compiler
CC = clang++

# Compiler flags
CFLAGS = -std=c++17 -Iinclude/GLFW

# Source files
SRC = src/main.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Output binary
OUT = bin/main

# Libraries
LDFLAGS = -Llibs -lglfw

# GLFW required frameworks on macOS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
    LDFLAGS += -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa
endif

# Build rule
$(OUT): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(OUT) $(LDFLAGS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OUT) $(OBJ)

.PHONY: clean
