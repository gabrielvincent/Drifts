#include <SDL/SDL.h>

#ifndef CUSTOMCURSOR_H_INCLUDED
#define CUSTOMCURSOR_H_INCLUDED

typedef struct {
	SDL_Surface *image;
	SDL_Rect frame;
} Cursor;

void moveCursor(Cursor *cursor, SDL_Surface *screen);

#endif