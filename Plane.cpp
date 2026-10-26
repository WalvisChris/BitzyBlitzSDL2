#include <cmath>
#include "Plane.h"
#include "Bullet.h"

Plane::Plane(float startX, float startY, int type, Player* player, SDL_Texture* texture) {
	this->x = startX;
	this->y = startY;
	this->vx = 0.0f;
	this->vy = 0.0f;
	this->tx = 30.0f;
	this->ty = 30.0f;
	this->frameWidth = 336 / 4;
	this->frameHeight = 36;
	this->degrees = 0;
	this->speed = 150.0f;
	this->turnSpeed = 150.0f;
	this->type = type;
	this->state = 0;
    this->shootTimer = 0.0f;
    this->shootCooldown = 700;
	this->player = player;
	this->spriteSheet = texture;
}

bool Plane::update(float deltaTime, float frameTime) {
    const float PI = 3.14159265f;

    if (shootTimer < shootCooldown) { shootTimer += frameTime; }

    switch (state) {
    case 0: { // find target
        tx = (player->x > 320.0f) ? 610.0f : 30.0f;
        ty = 30.0f;
        state = 1;
        break;
    }

    case 1: { // go to target
        float fx = tx - x;
        float fy = ty - y;

        float fradians = std::atan2(fy, fx);
        float fdegrees = fradians * (180.0f / PI);

        float fdiff = fdegrees - degrees;
        while (fdiff < -180.0f) fdiff += 360.0f;
        while (fdiff > 180.0f) fdiff -= 360.0f;

        if (fdiff > 0.0f) {
            degrees += turnSpeed * deltaTime;
        }
        else if (fdiff < 0.0f) {
            degrees -= turnSpeed * deltaTime;
        }

        if (std::abs(fdiff) <= turnSpeed * deltaTime) {
            degrees = fdegrees;
        }

        float fradians_current = degrees * (PI / 180.0f);

        vx = std::cos(fradians_current) * speed;
        vy = std::sin(fradians_current) * speed;

        x += vx * deltaTime;
        y += vy * deltaTime;

        float distance = std::sqrt((tx - x) * (tx - x) + (ty - y) * (ty - y));

        if (distance < 20.0f) {
            state = 2;
        }
        break;
    }

    case 2: { // shoot player
        float dx = player->x - x;
        float dy = player->y - y;

        float dradians = std::atan2(dy, dx);
        float ddegrees = dradians * (180.0f / PI);

        float ddiff = ddegrees - degrees;
        while (ddiff < -180.0f) ddiff += 360.0f;
        while (ddiff > 180.0f) ddiff -= 360.0f;

        if (ddiff > 0.0f) {
            degrees += turnSpeed * deltaTime;
        }
        else if (ddiff < 0.0f) {
            degrees -= turnSpeed * deltaTime;
        }

        if (std::abs(ddiff) <= turnSpeed * deltaTime) {
            degrees = ddegrees;
        }

        float dradians_current = degrees * (PI / 180.0f);

        vx = std::cos(dradians_current) * speed;
        vy = std::sin(dradians_current) * speed;

        x += vx * deltaTime;
        y += vy * deltaTime;

        if (y > 300.0f) {
            state = 0;
        }
        break;
    }

    default: {
        state = 0;
        break;
    }
    }

    return true; // alive
}

void Plane::render(SDL_Renderer* renderer) {
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

	SDL_Point center = { static_cast<int>(frameWidth / 2), static_cast<int>(frameHeight / 2)};
	SDL_RenderCopyEx(renderer, spriteSheet, &srcRect, &destRect, degrees, &center, SDL_FLIP_NONE);
}