#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

int main (int argc, char **argv) {

	int quit = 0;
	SDL_Event event;

	SDL_Surface *screen;
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
	    puts(SDL_GetError());
	    return 1;
	}

	screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE);

  while(!quit) {
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT)
				quit = 1;
		}

		SDL_Flip(screen);
	}

	SDL_Quit();
	return 0;
}