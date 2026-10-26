#pragma once
#include <SDL2/SDL.h>

class Balloon {
public:
	float x, y;
	float vx, vy;
	int type;
	int health, damage;

	int frameWidth, frameHeight;
	SDL_Texture* spriteSheet;

	Balloon(float startX, float startY, int type, SDL_Texture* texture);
	bool update(float deltaTime);
	void render(SDL_Renderer* renderer);
};