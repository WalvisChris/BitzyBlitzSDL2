#pragma once
#include <SDL2/SDL.h>

class Healthbox {
public:
	float x, y;
	float vy;
	float gravity;
	float width, height;
	SDL_Texture* texture;

	Healthbox(float startX, float startY, SDL_Texture* texture);
	void update(float deltaTime);
	void render(SDL_Renderer* renderer);
};