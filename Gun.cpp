#include <cmath>
#include "Gun.h"

Gun::Gun(Player* player, SDL_Texture* texture) {
    this->playerRef = player;
    this->x = playerRef->x;
    this->y = playerRef->y;
    this->width = 57.0f;
    this->height = 19.0f;
    this->angle = 0.0f;
    this->texture = texture;
}

void Gun::update() {
    x = playerRef->x + (playerRef->width / 2);
    y = playerRef->y + (playerRef->height / 2);
}

void Gun::render(SDL_Renderer* renderer, Sint16 rightX, Sint16 rightY) {
    const Sint16 deadzone = 8000;

    if (std::abs(rightX) > deadzone || std::abs(rightY) > deadzone) {
        float fx = static_cast<float>(rightX);
        float fy = static_cast<float>(rightY);
        const float PI = 3.14159265f;
        float radians = std::atan2(fy, fx);
        angle = radians * (180.0f / PI);
    }

    SDL_Rect destRect = {
        static_cast<int>(x),
        static_cast<int>(y),
        static_cast<int>(width),
        static_cast<int>(height)
    };

    SDL_Point center = { 0, static_cast<int>(height / 2) };
    SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, angle, &center, SDL_FLIP_NONE);
}