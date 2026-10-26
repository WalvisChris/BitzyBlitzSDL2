#include <cmath>
#include "Healthbar.h"

Healthbar::Healthbar(int HP, int maxHP, SDL_Texture* texture) {
    this->health = HP;
    this->maxHealth = maxHP;
    this->frameWidth = 145 / 5;
    this->frameHeight = 27;
    this->texture = texture;
}

void Healthbar::update() {
    return;
}

void Healthbar::render(SDL_Renderer* renderer) {  
    int fullHearts = health / 4;
    int remainder = health % 4;
    int totalHearts = (maxHealth / 4) + ((maxHealth % 4) > 0);
    int emptyHearts = totalHearts - fullHearts - (remainder > 0);

    int renderIndexX = 0;
    
    for (int fh = 0; fh < fullHearts; ++fh) {
        renderHeart(renderer, 4, renderIndexX);
        ++renderIndexX;
    }

    if (remainder > 0) { renderHeart(renderer, remainder, renderIndexX); ++renderIndexX; }

    for (int eh = 0; eh < emptyHearts; ++eh) {
        renderHeart(renderer, 0, renderIndexX);
        ++renderIndexX;
    }
}

void Healthbar::renderHeart(SDL_Renderer* renderer, int col, int renderIndexX) {
    SDL_Rect srcRect = {
        col * frameWidth,
        0,
        frameWidth,
        frameHeight
    };

    const int spacing = 2;
    SDL_Rect destRect = {
        5 + (renderIndexX * (frameWidth + spacing)),
        5,
        frameWidth,
        frameHeight
    };

    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}