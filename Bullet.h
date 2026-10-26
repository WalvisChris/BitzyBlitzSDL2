#pragma once
#include <SDL2/SDL.h>

class Bullet {
public:
	float x, y;
	float vx, vy;
	double angle;
	int frameWidth, frameHeight;
	int type, damage;
	SDL_Texture* texture;

	Bullet(float startX, float startY, float startVX, float startVY, double angle, int bulletType, SDL_Texture* texture);
	bool update(float deltaTime);
	void render(SDL_Renderer* renderer);
};