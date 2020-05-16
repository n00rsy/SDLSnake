#include "iostream"
#include "SDL.h"
#include <stdlib.h>
#include <ctime>

#define WIN_X 1024
#define WIN_Y 512

const int ITEM_SIZE = WIN_Y / 32;
const int MAP_X = WIN_X / ITEM_SIZE;
const int MAP_Y = WIN_Y / ITEM_SIZE;

enum cell{ blank, snake, food };

typedef struct head {
	int x;
	int y;
	enum direction { right = 1, left, up, down } d;
};

bool init(SDL_Window**, SDL_Renderer **, cell[MAP_X][MAP_Y]);
void renderWorld(cell[MAP_X][MAP_Y], SDL_Renderer**);

int main(int argc, char* argv[])
{
	SDL_Window *window;
	SDL_Renderer* renderer;
	cell map[MAP_X][MAP_Y];
	head* sHead;
	sHead->x = MAP_X / 2;
	sHead->y = MAP_Y / 2;
	sHead->d = head::right;

	//init SDL stuff and game world
	if (!init(&window, &renderer,map)) {
		printf("Error initializing\n");
		return -1;
	}
	renderWorld(map, &renderer);

	SDL_Delay(3000); // window lasts 3 seconds
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

void renderWorld(cell map[MAP_X][MAP_Y], SDL_Renderer** renderer) {
	int x, y;
	for (x = 0; x < MAP_X; x++) {
		for (y = 0; y < MAP_Y; y++) {
			if (map[x][y] != blank) {
				switch (map[x][y]) {
				case snake:
					SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
					break;
				case food:
					SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255);
					break;
				}
				SDL_Rect rect;
				rect.x = x * ITEM_SIZE;
				rect.y = y * ITEM_SIZE;
				rect.w = ITEM_SIZE;
				rect.h = ITEM_SIZE;
				SDL_RenderFillRect(*renderer, &rect);
			}
		}
	}
	SDL_RenderPresent(*renderer);
}

void getNewFoodPos(cell map[MAP_X][MAP_Y]) {

	int pos[2] = { rand() % MAP_X ,rand() % MAP_Y };

	while (map[pos[0]][pos[1]] != blank) {
		pos[0] = rand() % MAP_X;
		pos[1] = rand() % MAP_Y;
	}
	map[pos[0]][pos[1]] = food;
}

bool init(SDL_Window ** window, SDL_Renderer** renderer, cell map[MAP_X][MAP_Y]) {
	SDL_Init(SDL_INIT_VIDEO);
		*window = SDL_CreateWindow
	("SDL Snake", 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		WIN_X, WIN_Y, 
		SDL_WINDOW_OPENGL);

	if (*window == NULL) {
		printf("%s\n", SDL_GetError());
		return false;
	}
	*renderer = SDL_CreateRenderer(*window, -1, 0);
	int x, y;
	for (x = 0; x < MAP_X; x++) {
		for (y = 0; y < MAP_Y; y++) {
			map[x][y] = blank;
		}
	}
	map[MAP_X / 2][MAP_Y / 2] = snake;
	map[(MAP_X / 2) + 1][MAP_Y / 2] = snake;
	srand(time(NULL));
	getNewFoodPos(map);
	return true;
}