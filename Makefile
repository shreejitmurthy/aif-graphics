# Compiler
CC = clang++

# Compiler flags
CFLAGS = -std=c++17 -Iinclude/GLFW -IEngine/glad

# Source files
SRC = example/src/main.cpp Engine/vendor/glad/glad.c Engine/src/graphics.cpp

# Object files
OBJ = $(SRC:example/src/%.cpp=example/bin/%.o)
# OBJ := $(OBJ:Engine/vendor/include/glad/%.c=Engine/bin/%.o)

# Output binary
OUT = example/bin/main

# Libraries
LDFLAGS = -Lexample/libs -lglfw

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
example/bin/%.o: example/src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Engine/bin/%.o: example/include/glad/%.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# Run rule
run: $(OUT)
	./$(OUT)

# Clean rule
clean:
	rm -f $(OUT) example/bin/*.o

.PHONY: clean run
