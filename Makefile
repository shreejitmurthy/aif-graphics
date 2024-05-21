# Compiler
CC = clang++

# Compiler flags
CFLAGS = -std=c++17 -Iinclude/GLFW -Iinclude/glad

# Source files
SRC = src/main.cpp include/glad/glad.c src/graphics.cpp

# Object files
OBJ = $(SRC:src/%.cpp=bin/%.o)
OBJ := $(OBJ:include/glad/%.c=bin/%.o)

# Output binary
OUT = bin/main

# Libraries
LDFLAGS = -Llibs -lglfw

# GLFW required frameworks on macOS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
    LDFLAGS += -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa
endif

# Ensure bin directory exists
$(shell mkdir -p bin)

# Build rule
$(OUT): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(OUT) $(LDFLAGS)

# Compile source files to object files
bin/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

bin/%.o: include/glad/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run rule
run: $(OUT)
	./$(OUT)

# Clean rule
clean:
	rm -f $(OUT) bin/*.o

.PHONY: clean run
