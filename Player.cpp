#include <cmath>
#include "Player.h"

Player::Player(float startX, float startY, SDL_Texture* texture) {
    this->x = startX;
    this->y = startY;
    this->vx = 0.0f;
    this->vy = 0.0f;
    this->width = 64.0f;
    this->height = 64.0f;
    this->drag = 5.0f;
    this->gravity = 1000.0f;
    this->jumpHeight = 400.0f;
    this->facingLeft = false;

    this->health = 4;
    this->maxHealth = 12;

    spriteSheet = texture;
    currentFrame = 0;
}

void Player::update(float deltaTime) {
    vx -= vx * drag * deltaTime;
    vy += gravity * deltaTime;

    x += vx * deltaTime;
    y += vy * deltaTime;

    if (x < 0.0f) { x = 0.0f; vx = 0.0f; }
    if (x + width > 640.0f) { x = 640.0f - width; vx = 0.0f; }
    if (y > 450.0f - height) { y = 450.0f - height; vy = 0.0f; }

    if (facingLeft && vx > 0.0f) { facingLeft = false;  }
    else if (!facingLeft && vx < 0.0f) { facingLeft = true;  }
}

void Player::render(SDL_Renderer* renderer) {
    currentFrame = 0;
    if (std::abs(vy) > 0.1f) {
        const float threshold = 100.0f;
        if (std::abs(vy) < threshold) { currentFrame = 7; }
        else if (vy > threshold) { currentFrame = 8; }
        else { currentFrame = 6; }
    }
    else if (std::abs(vx) > 0.1f) { currentFrame = std::abs(static_cast<int>(std::round(x / 20.0f))) % 6; }

    int frameWidth = 384 / 6;
    int frameHeight = 128 / 2;
    int col = currentFrame % 6;
    int row = currentFrame / 6;

    SDL_Rect srcRect = {
        col * frameWidth,
        row * frameHeight,
        frameWidth,
        frameHeight
    };

    SDL_Rect destRect = {
        static_cast<int>(x),
        static_cast<int>(y),
        static_cast<int>(width),
        static_cast<int>(height)
    };
    
    SDL_RendererFlip flip = facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer, spriteSheet, &srcRect, &destRect, 0.0, nullptr, flip);
}