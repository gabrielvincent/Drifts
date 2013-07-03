# gcc -o Drifts main.c -Wall `sdl-config --cflags --libs` -lSDLmain -lSDL_image

CC = gcc
CFLAGS = -Wall -o
SDLFLAGS = -lSDLmain -lSDL_image

all: main.o
	$(CC) main.o $(CFLAGS) Drifts `sdl-config --cflags --libs` $(SDLFLAGS)

main.o: main.c
	$(CC) -o main.o -c main.c $(CFLAGS) `sdl-config --cflags --libs` $(SDLFLAGS)