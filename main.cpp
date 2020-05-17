#include "iostream"
#include "SDL.h"
#include <stdlib.h>
#include <ctime>

#define WIN_X 1024
#define WIN_Y 512

const int ITEM_SIZE = WIN_Y / 32;
const int MAP_X = WIN_X / ITEM_SIZE;
const int MAP_Y = WIN_Y / ITEM_SIZE;

const int FRAMES_PER_SECOND = 4;

enum cell { blank, body, food };

typedef struct _snakeNode {
	int x;
	int y;
	_snakeNode* next;
}snakeNode;

typedef struct _snake {
	enum direction { right, left, up, down } d;
	snakeNode* head;
}snake;

bool init(SDL_Window**, SDL_Renderer**, cell[MAP_X][MAP_Y], snake*);
bool update(cell[MAP_X][MAP_Y], snake*);
void renderWorld(cell[MAP_X][MAP_Y], SDL_Renderer**);
void getNewFoodPos(cell[MAP_X][MAP_Y]);

int main(int argc, char* argv[]) {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	cell map[MAP_X][MAP_Y];
	bool running = true;
	snake* gSnake = (snake*)malloc(sizeof(gSnake));
	gSnake->d = snake::right;
	//init SDL stuff and game world
	if (!init(&window, &renderer, map, gSnake)) {
		printf("Error initializing\n");
		return -1;
	}
	while (running) {
		//handle events
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_LEFT:
					if (gSnake->d != snake::right)gSnake->d = snake::left;
					break;
				case SDLK_RIGHT:
					if (gSnake->d != snake::left)gSnake->d = snake::right;
					break;
				case SDLK_UP:
					if (gSnake->d != snake::down)gSnake->d = snake::up;
					break;
				case SDLK_DOWN:
					if (gSnake->d != snake::up)gSnake->d = snake::down;
					break;
				default:
					break;
				}
			}
		}

		//perform game logic and update snake body positions 
		if (running = update(map, gSnake)) {
			//render map
			renderWorld(map, &renderer);
		}
		SDL_Delay(100); 
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

void clearSnakeFromMap(cell map[MAP_X][MAP_Y], snake* gSnake) {
	snakeNode* ptr = gSnake->head;
	while (ptr != NULL) {
		map[ptr->x][ptr->y] = blank;
		ptr = ptr->next;
	}
}

void putSnakeOnMap(cell map[MAP_X][MAP_Y], snake* gSnake) {
	snakeNode* ptr = gSnake->head;
	while (ptr != NULL) {
		map[ptr->x][ptr->y] = body;
		ptr = ptr->next;
	}
}


bool update(cell map[MAP_X][MAP_Y], snake* gSnake) {
	int newHeadPos[2] = { gSnake->head->x,gSnake->head->y };
	switch (gSnake->d) {
	case snake::up:
		newHeadPos[1] -= 1;
		break;
	case snake::down:
		newHeadPos[1] += 1;
		break;
	case snake::right:
		newHeadPos[0] += 1;
		break;
	case snake::left:
		newHeadPos[0] -= 1;
		break;
	}
	//if head is outside world bounds
	if (newHeadPos[0]<0 || newHeadPos[0]>MAP_X - 1 ||
		newHeadPos[1]<0 || newHeadPos[1]>MAP_Y - 1) {
		return false;
	}
	//if head collides with snake
	if (map[newHeadPos[0]][newHeadPos[1]] == body) {
		return false;
	}
	clearSnakeFromMap(map, gSnake);
	if (map[newHeadPos[0]][newHeadPos[1]] == food) {
		getNewFoodPos(map);
		//add node to end of llist
		snakeNode* ptr = gSnake->head;
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = (snakeNode*)malloc(sizeof(snakeNode));
		ptr->next->next = NULL;
	}

	//update body pos
	snakeNode* ptr = gSnake->head->next;
	int x=gSnake->head->x;
	int y = gSnake->head->y;
	int xTemp, yTemp;
	//iterate through list and move each node to its parent node's pos
	while (ptr != NULL) {
		xTemp = ptr->x;
		yTemp = ptr->y;
		ptr->x = x;
		ptr->y = y;
		x = xTemp;
		y = yTemp;
		ptr = ptr->next;
	}

	gSnake->head->x = newHeadPos[0];
	gSnake->head->y = newHeadPos[1];
	putSnakeOnMap(map, gSnake);
	return true;
}

void renderWorld(cell map[MAP_X][MAP_Y], SDL_Renderer** renderer) {
	SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 0);
	SDL_RenderClear(*renderer);
	int x, y;
	for (x = 0; x < MAP_X; x++) {
		for (y = 0; y < MAP_Y; y++) {
			if (map[x][y] != blank) {
				switch (map[x][y]) {
				case body:
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

bool init(SDL_Window** window, SDL_Renderer** renderer, cell map[MAP_X][MAP_Y], snake* gSnake) {
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
	//create snake head
	
	//god damn this is ugly
	snakeNode* head = (snakeNode*)malloc(sizeof(snakeNode));
	snakeNode* second = (snakeNode*)malloc(sizeof(snakeNode));
	snakeNode* third = (snakeNode*)malloc(sizeof(snakeNode));
	snakeNode* fourth = (snakeNode*)malloc(sizeof(snakeNode));
	snakeNode* fifth = (snakeNode*)malloc(sizeof(snakeNode));
	snakeNode* sixth = (snakeNode*)malloc(sizeof(snakeNode));

	head->next = second; 
	second->next = third;
	third->next = fourth;
	fourth->next = fifth;
	fifth->next = sixth;
	sixth->next = NULL;

	snakeNode* ptr = head;
	//assign all nodes center starting position
	while (ptr != NULL) {
		ptr->x = MAP_X / 2;
		ptr->y = MAP_Y / 2;
		ptr = ptr -> next;
	}
	gSnake->head = head;
	srand(time(NULL));
	getNewFoodPos(map);
	return true;
}