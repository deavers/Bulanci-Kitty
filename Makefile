# Makefile

CC = gcc

CFLAGS = -Wall -Wextra -g `sdl2-config --cflags` -fsanitize=address

LIBS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

SRC = $(wildcard *.c)

OBJ = $(SRC:.c=.o)

EXEC = Bulanci

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
