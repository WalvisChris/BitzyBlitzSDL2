#pragma once
#include <SDL2/SDL.h>

class Bullet {
public:
	float x, y;
	float vx, vy;
	float speed;
	double angle;
	bool isFriendly;

	int frameWidth, frameHeight;
	int type, damage;
	SDL_Texture* texture;

	Bullet(float startX, float startY, double angle, int bulletType, bool isFriendly, SDL_Texture* texture);
	bool update(float deltaTime);
	void render(SDL_Renderer* renderer);
};