#include <SDL2/SDL.h>
#include "Player.h"

class Healthbar {
public:
    int health;
    int maxHealth;
    int frameWidth, frameHeight;
    SDL_Texture* texture;

    Healthbar(int health, int maxHealth, SDL_Texture* texture);
    void update();
    void render(SDL_Renderer* renderer);
    void renderHeart(SDL_Renderer* renderer, int col, int renderIndexX);
};