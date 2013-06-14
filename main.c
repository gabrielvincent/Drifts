#define COLLECTIBLE_BALL_FILENAME "CollectibleBall.png"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define X_AXIS 0
#define Y_AXIS 1

#define MAX_BALLS 200

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

typedef struct {
	SDL_Surface *image;
	SDL_Rect frame;
	float horizontalVelocity, verticallVelocity;
} Ball;

void changeDirection(Ball *ball, int axis) {

	switch (axis) {

		case X_AXIS:
			ball->horizontalVelocity = -ball->horizontalVelocity;
			break;

		case Y_AXIS:
			ball->verticallVelocity = -ball->verticallVelocity;
			break;
	}
}

void move(Ball *ball) {

	ball->frame.x += ball->horizontalVelocity;
	ball->frame.y += ball->verticallVelocity;

	if (ball->frame.x >= (SCREEN_WIDTH - ball->frame.w) || ball->frame.x <= 0)
		changeDirection(ball, X_AXIS);
	if (ball->frame.y >= (SCREEN_HEIGHT - ball->frame.h) || ball->frame.y <= 0)
		changeDirection(ball, Y_AXIS);
}

SDL_Rect SDL_RectMake(Sint16 x, Sint16 y, Uint16 w, Uint16 h) {

	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	return rect;
}

int surfaceWithFrameDidHitWallAtAxis(SDL_Rect *frame, int axis) {

	Sint16 x = frame->x;
	Sint16 y = frame->y;

	Uint16 w = frame->w;
	Uint16 h = frame->h;

	printf("x: %d | y: %d | w: %d | h:%d\n", x, y, w, h);

	switch (axis) {

		case X_AXIS:
			return (x >= SCREEN_WIDTH - w);

		case Y_AXIS:
			return (y >= SCREEN_HEIGHT - h);

		default:
			return 0;
	}
}

void moveCursor(Ball *cursor, SDL_Surface *screen) {

	Sint16 *x = &cursor->frame.x;
	Sint16 *y = &cursor->frame.y;

	Uint16 cursorWidth = cursor->frame.w;
	Uint16 cursorHeight = cursor->frame.h;

	SDL_GetMouseState((int *)x, (int *)y);

	if (surfaceWithFrameDidHitWallAtAxis(&cursor->frame, X_AXIS))
		*x = SCREEN_WIDTH - cursorWidth;
	if (surfaceWithFrameDidHitWallAtAxis(&cursor->frame, Y_AXIS))
		*y = SCREEN_HEIGHT - cursorHeight;

	SDL_BlitSurface(cursor->image, NULL, screen, &cursor->frame);
}

int main (int argc, char **argv) {

	SDL_Surface *screen;
	SDL_Event event;
	Uint32 backgroundColour;
	int quit = 0;

	Ball cursor;

	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
	    puts(SDL_GetError());
	    return 1;
	}

	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_SWSURFACE);
	backgroundColour = SDL_MapRGB(screen->format, 255, 255, 255); // White colour
	SDL_ShowCursor(SDL_DISABLE);

	cursor.frame = SDL_RectMake(0, 0, 40, 40);
	if (!(cursor.image = IMG_Load(COLLECTIBLE_BALL_FILENAME))) {
		printf("Could not load image %s\nSDL ERROR %s\n", COLLECTIBLE_BALL_FILENAME, SDL_GetError());
		exit(1);
	}

  while(!quit) {
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT)
				quit = 1;
		}

		SDL_FillRect(screen, NULL, backgroundColour);

		moveCursor(&cursor, screen);

		SDL_Flip(screen);
	}

	SDL_Quit();
	return 0;
}