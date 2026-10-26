#include <cmath>
#include "Bullet.h"

Bullet::Bullet(float startX, float startY, float startVX, float startVY, double angle, int bulletType, SDL_Texture* texture) {
    this->x = startX;
    this->y = startY;
    this->vx = startVX;
    this->vy = startVY;
    this->angle = angle;
    this->type = bulletType;
    this->frameWidth = 96 / 6;
    this->frameHeight = 16;
    this->texture = texture;
}

bool Bullet::update(float deltaTime) {
    x += vx * deltaTime;
    y += vy * deltaTime;

    if (x < 0 || x > 640.0f || y < 0 || y > 480.0f) {
        return false; // dead
    }

    return true; // alive
}

void Bullet::render(SDL_Renderer* renderer) {
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

    SDL_Point center = { 0, static_cast<int>(frameHeight / 2) };
    SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, angle, &center, SDL_FLIP_NONE);
}