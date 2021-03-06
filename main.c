#define COLLECTIBLE_BALL_FILENAME "CollectibleBall.png"
#define CURSOR_FILENAME "Cursor.png"

#define MAXIMUM_FPS 24
#define MAXIMUM_DISPATCH_INTERVAL_MODULE 3
#define MAXIMUM_NUMBER_OF_BALLS_DISPATCHED 5
#define MAXIMUM_HORIZONTAL_VELOCITY 10
#define MAXIMUM_VERTICAL_VELOCITY 2
#define MAXIMUM_BALLS 151

#define SCREEN_WIDTH  640.0
#define SCREEN_HEIGHT 480.0

#define X_AXIS 0
#define Y_AXIS 1

#define NO  0
#define YES 1

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#pragma mark - Typedefs

typedef char BOOL;

typedef struct {
	SDL_Surface *surface;
	SDL_Rect frame;
} Buffer;

typedef struct {
	SDL_Surface *image;
	SDL_Rect frame;
	float horizontalVelocity, verticallVelocity;
	BOOL visible;
} Ball;

typedef struct {
	SDL_Surface *image;
	SDL_Rect frame;
} Cursor;

Cursor cursor;
Buffer buffer;
float lastDispatchedTicks = 0.0;
float nextDispatchInterval = 0.0;

#pragma mark - Prototypes

SDL_Rect SDL_RectMake(Sint16 x, Sint16 y, Uint16 w, Uint16 h);
void moveBall(Ball *ball);

#pragma mark - Setups

void prepareCollectibleBallForReuse(Ball *collectibleBall) {

	float randomOriginX = SCREEN_WIDTH / 2.0;
	float randomOriginY = -40;

	collectibleBall->frame = SDL_RectMake(randomOriginX, randomOriginY, 40, 40);
	collectibleBall->horizontalVelocity = rand() % MAXIMUM_HORIZONTAL_VELOCITY;
	collectibleBall->verticallVelocity = 1 + rand() % MAXIMUM_VERTICAL_VELOCITY;
	collectibleBall->visible = NO;

	if (rand() % 2)
		collectibleBall->horizontalVelocity *= -1;

	// printf("\nX: %f | Y: %f | Hor: %f | Ver: %f", randomOriginX, randomOriginY, collectibleBall->horizontalVelocity, collectibleBall->verticallVelocity);
}

void setupCollectibleBalls(Ball *collectibleBalls) {

	int i;

	for (i = 0; i < MAXIMUM_BALLS; i++) {

		if (!((collectibleBalls[i]).image = IMG_Load(COLLECTIBLE_BALL_FILENAME))) {
			printf("SDL ERROR: %s\n", SDL_GetError());
			exit(1);
		}

		prepareCollectibleBallForReuse(&collectibleBalls[i]);
	}
}

#pragma mark - Implementation

void dispatchBalls(Ball *collectibleBalls, SDL_Surface *buffer) {
    
	float lastDispatchedInterval = SDL_GetTicks()	- lastDispatchedTicks;

	int i = 0;
	int j = 0;

  if (lastDispatchedInterval >= nextDispatchInterval) {

  	int numberOfBallsToAppearEachInterval = 1 + rand() % MAXIMUM_NUMBER_OF_BALLS_DISPATCHED;

    while (j < numberOfBallsToAppearEachInterval) {

    	if (!(collectibleBalls[i]).visible) {

    		(collectibleBalls[i]).visible = YES;
    		j++;
    	}

    	i++;
    }
 		
   	lastDispatchedTicks = SDL_GetTicks(); 
   	nextDispatchInterval = (1 + rand() % 3) * 1000; // Interval expressed in miliseconds

    return;
  }

  for (i = 0; i < MAXIMUM_BALLS; i++)
  	if ((collectibleBalls[i]).visible)
  		moveBall(&collectibleBalls[i]);
}

void moveBall(Ball *ball) {

	ball->frame.x += ball->horizontalVelocity;
	ball->frame.y += ball->verticallVelocity;

	SDL_Rect temporaryFrame = ball->frame;

	if (ball->frame.x >= SCREEN_WIDTH || ball->frame.x <= -ball->frame.w || ball->frame.y >= SCREEN_HEIGHT)
		prepareCollectibleBallForReuse(ball);

	SDL_BlitSurface(ball->image, NULL, buffer.surface, &temporaryFrame);
}

/*
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
*/

SDL_Color SDL_ColorMake(Uint8 red, Uint8 green, Uint8 blue) {

	SDL_Color colour;
	colour.r = red;
	colour.g = green;
	colour.b = blue;

	return colour;
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

	switch (axis) {

		case X_AXIS:
			return (x >= SCREEN_WIDTH - w);

		case Y_AXIS:
			return (y >= SCREEN_HEIGHT - h);

		default:
			return 0;
	}
}

void moveCursor() {

	int x;
	int y;

	SDL_GetMouseState(&x, &y);

	cursor.frame.x = x;
	cursor.frame.y = y;

	SDL_BlitSurface(cursor.image, NULL, buffer.surface, &cursor.frame);
}

int main (int argc, char **argv) {

	srand(time(NULL));

	SDL_Surface *screen;
	SDL_Event event;
	Uint32 backgroundColour;
	int quit = 0;

	Ball collectibleBalls[MAXIMUM_BALLS];

	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
	    puts(SDL_GetError());
	    exit(1);
	}
    
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_SWSURFACE);
	backgroundColour = SDL_MapRGB(screen->format, 255, 255, 255); // White colour
	SDL_ShowCursor(SDL_DISABLE);

	buffer.surface = screen;
	buffer.frame = SDL_RectMake(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	setupCollectibleBalls(collectibleBalls);

	cursor.frame = SDL_RectMake(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 40, 40);
	if (!(cursor.image = IMG_Load(CURSOR_FILENAME))) {
		printf("SDL ERROR: %s\n", SDL_GetError());
		exit(1);
	}

  while(!quit) {
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					quit = 1;
					break;

				default:
					break;
			}
		}

		if (SDL_GetTicks() % MAXIMUM_FPS == 0) {

			SDL_FillRect(buffer.surface, NULL, backgroundColour);

			moveCursor();
			dispatchBalls(collectibleBalls, buffer.surface);

			SDL_BlitSurface(buffer.surface, NULL, screen, &buffer.frame); // Commits all previous blits with a single blit to the screen
			SDL_Flip(screen);
		}
	}

	SDL_Quit();
	return 0;
}