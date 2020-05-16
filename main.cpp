#include "iostream"
#include "SDL.h"

bool init(SDL_Window**, SDL_Renderer **);

int main(int argc, char* argv[])
{
	SDL_Window *window;
	SDL_Renderer* renderer;
	//init SDL stuff and game world
	if (!init(&window, &renderer)) {
		printf("Error initializing\n");
		return -1;
	}


	SDL_Rect rect;
	rect.x = 250;
	rect.y = 150;
	rect.w = 200;
	rect.h = 200;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &rect);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_RenderPresent(renderer);


	SDL_Delay(3000); // window lasts 3 seconds
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

bool init(SDL_Window ** window, SDL_Renderer** renderer) {
	SDL_Init(SDL_INIT_VIDEO);
		*window = SDL_CreateWindow
	("SDL Snake", 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		640, 480, 
		SDL_WINDOW_OPENGL);

	if (*window == NULL) {
		printf("%s\n", SDL_GetError());
		return false;
	}
	*renderer = SDL_CreateRenderer(*window, -1, 0);
	return true;
}