CC = gcc

CFLAGS = -Wall -Wextra -O2 `sdl2-config --cflags` -Iinclude

LIBS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

SRC_DIR = src
OBJ_DIR = build
INC_DIR = include

SRC = $(wildcard $(SRC_DIR)/*.c)

OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

EXEC = Bulanci

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(EXEC)

.PHONY: all clean