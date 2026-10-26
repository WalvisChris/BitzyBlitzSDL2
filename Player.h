#pragma once
#include <SDL2/SDL.h>

class Player {
public:
    float x, y;
    float vx, vy;
    float width, height;
    float drag, gravity;
    float floorY;
    float jumpHeight;
    int health, maxHealth;
    bool facingLeft;
    
    SDL_Texture* spriteSheet;
    int currentFrame;

    Player(float startX, float startY, float floorY, SDL_Texture* texture);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
};