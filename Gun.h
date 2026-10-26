#pragma once
#include <SDL2/SDL.h>
#include "Player.h"

class Gun {
public:
    float x, y;
    float width, height;
    double angle;
    Player* playerRef;
    SDL_Texture* texture;

    Gun(Player* player, SDL_Texture* texture);
    void update();
    void render(SDL_Renderer* renderer, Sint16 rightX, Sint16 rightY);
};