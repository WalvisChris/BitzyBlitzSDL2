#include <cmath>
#include "Bullet.h"

Bullet::Bullet(float startX, float startY, double angle, int bulletType, bool isFriendly, SDL_Texture* texture) {
    this->x = startX;
    this->y = startY;
    this->angle = angle;
    this->speed = 500.0f;

    const float PI = 3.14159265f;
    float radians = angle * (PI / 180.0f);
    this->vx = std::cos(radians) * speed;
    this->vy = std::sin(radians) * speed;
    
    this->type = bulletType;
    this->damage = 1 + (2 * bulletType);
    this->isFriendly = isFriendly;

    this->frameWidth = 96 / 6;
    this->frameHeight = 16;
    this->texture = texture;
}

bool Bullet::update(float deltaTime) {   
    x += vx * deltaTime;
    y += vy * deltaTime;

    if (x < 0 || x > 640.0f - frameWidth|| y < 0 || y > 450.0f - frameHeight) {
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