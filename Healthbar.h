#pragma once
#include <SDL2/SDL.h>
#include "Player.h"

class Healthbar {
public:
    int health;
    int maxHealth;
    int frameWidth, frameHeight;

    Player* player;
    SDL_Texture* texture;

    Healthbar(Player* player, SDL_Texture* texture);
    void update();
    void render(SDL_Renderer* renderer);
    void renderHeart(SDL_Renderer* renderer, int col, int renderIndexX);
};