#include "Balloon.h"

Balloon::Balloon(float startX, float startY, int type, SDL_Texture* texture) {
	this->x = startX;
	this->y = startY;
	this->vx = 100.0f;
	this->vy = 0.0f;
	this->frameWidth = 236 / 4;
	this->frameHeight = 82;
	this->type = type;
	this->health = (type + 1) * 5;
	this->damage = (type + 1) * 2;
	this->spriteSheet = texture;
}

bool Balloon::update(float deltaTime) {
	x += vx * deltaTime;
	y += vy * deltaTime;

	if (x < 0.0f || x > 640.0f - frameWidth) { vx *= -1; x += vx * deltaTime; }

	if (health <= 0) { return false; } // dead

	return true; // alive
}

void Balloon::render(SDL_Renderer* renderer) {
	SDL_Rect srcRect = {
		type * frameWidth,
		0,
		frameWidth,
		frameHeight
	};

	SDL_Rect destRect = {
		x,
		y,
		frameWidth,
		frameHeight
	};

	SDL_RenderCopy(renderer, spriteSheet, &srcRect, &destRect);
}