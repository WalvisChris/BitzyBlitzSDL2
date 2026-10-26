#include "Healthbox.h"

Healthbox::Healthbox(float startX, float startY, SDL_Texture* texture) {
	this->x = startX;
	this->y = startY;
	this->vy = 0.0f;
	this->gravity = 1000.0f;
	this->width = 33.0f;
	this->height = 33.0f;
	this->texture = texture;
}

void Healthbox::update(float deltaTime) {
	vy += gravity * deltaTime;
	y += vy * deltaTime;

	if (y > 450.0f - height) { y = 450.0f - height; vy = 0.0f; }
}

void Healthbox::render(SDL_Renderer* renderer) {
	SDL_Rect destRect = {
		x,
		y,
		width,
		height
	};

	SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}