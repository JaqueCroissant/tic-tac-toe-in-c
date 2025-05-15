# Compiler
CC = gcc

# Source and output
SRC = main.c
OUT = tic-tac-toe.exe

# Compiler and linker flags
CFLAGS = -O1 -Wall -std=c99 -Wno-missing-braces -Iinclude/
LDFLAGS = -Llib/ -lraylib -lopengl32 -lgdi32 -lwinmm

# Default target
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS)

# Clean target
clean:
	del /Q $(OUT)