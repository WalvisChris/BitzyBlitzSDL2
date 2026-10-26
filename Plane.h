#pragma once
#include <SDL2/SDL.h>
#include "Player.h"

class Plane {
public:
	float x, y; // position
	float vx, vy; // velocity
	float tx, ty; // target
	float frameWidth, frameHeight;
	double degrees;
	float speed, turnSpeed;
	int type, state;
	float shootTimer;
	Uint32 shootCooldown;
	Player* player;
	SDL_Texture* spriteSheet;

	Plane(float startX, float startY, int type, Player* player, SDL_Texture* texture);
	bool update(float deltaTime, float frameTime);
	void render(SDL_Renderer* renderer);
};