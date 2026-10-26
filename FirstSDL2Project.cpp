#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include "Player.h"
#include "Gun.h"
#include "Healthbar.h"
#include "Bullet.h"

int main(int argc, char* argv[]) {

    const int screenWidth = 640;
    const int screenHeight = 480;
    const float floorY = screenHeight - 30.0f;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL_image: %s", IMG_GetError());
        SDL_Quit();
        return 3;
    }

    SDL_Window* window = SDL_CreateWindow("SDL2 Bitzy Blitz", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create window: %s", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 3;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 3;
    }

    SDL_GameController* controller = nullptr;
    if (SDL_NumJoysticks() > 0) {
        if (SDL_IsGameController(0)) {
            controller = SDL_GameControllerOpen(0);
            SDL_Log("Controller connected: %s", SDL_GameControllerName(controller));
        }
    }

    SDL_Surface* playerSurface = IMG_Load("Media/horse-sheet.png");
    SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);

    Player* player = new Player(320.0f, 240.0f, floorY, playerTexture);

    SDL_Surface* gunSurface = IMG_Load("Media/gun.png");
    SDL_Texture* gunTexture = SDL_CreateTextureFromSurface(renderer, gunSurface);
    SDL_FreeSurface(gunSurface);

    Gun* gun = new Gun(player, gunTexture);

    SDL_Surface* bulletSurface = IMG_Load("Media/bullet-sheet.png");
    SDL_Texture* bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletSurface);
    SDL_FreeSurface(bulletSurface);

    SDL_Surface* healthbarSurface = IMG_Load("Media/health-sheet.png");
    SDL_Texture* healthbarTexture = SDL_CreateTextureFromSurface(renderer, healthbarSurface);
    SDL_FreeSurface(healthbarSurface);

    Healthbar* healthbar = new Healthbar(10, 12, healthbarTexture);

    SDL_Surface* bgSurface = IMG_Load("Media/background.png");
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    std::vector<Bullet> bullets;
    bool r2Flag = false;
    bool isRunning{ true };
    SDL_Event event;

    Uint32 lastTime = SDL_GetTicks();

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        /*const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_LEFT]) {
            player.vx = -250.0f;
        }
        if (keystate[SDL_SCANCODE_RIGHT]) {
            player.vx = 250.0f;
        }

        if (keystate[SDL_SCANCODE_UP] && player.y >= floorY - player.height) {
            player.vy = -player.jumpHeight;
        }*/

        player->vx = 0.0f;

        Sint16 rightX = 0;
        Sint16 rightY = 0;

        if (controller != nullptr) {
            Sint16 leftX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
            const Sint16 deadzone = 8000;
            float normalizedLeftX = static_cast<float>(leftX) / 32768.0f;
            if (leftX < -deadzone) { player->vx = 250.0f * normalizedLeftX; }
            else if (leftX > deadzone) { player->vx = 250.0f * normalizedLeftX; }

            Sint16 l2Value = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
            if (l2Value > 1000 && player->y >= floorY - player->height) { player->vy = -player->jumpHeight; }

            rightX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
            rightY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);

            Sint16 r2Value = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

            if (r2Value > 1000 && !r2Flag) {
                const float PI = 3.14159265f;
                float radians = gun->angle * (PI / 180.0f);

                float dx = std::cos(radians);
                float dy = std::sin(radians);

                float speed = 500.0f;
                float startVX = dx * speed;
                float startVY = dy * speed;

                float gunSteps = 40.0f;
                float playerCenterX = player->x + player->width / 2;
                float playerCenterY = player->y + player->height / 2;
                float startX = playerCenterX + dx * gunSteps;
                float startY = playerCenterY + dy * gunSteps;

                bullets.push_back(Bullet(startX, startY, startVX, startVY, gun->angle, 4, bulletTexture));

                r2Flag = true;
            }

            if (r2Flag && r2Value < 1000) {
                r2Flag = false;
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        player->update(deltaTime);
        gun->update();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (bgTexture != nullptr) {
            SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
        }

        player->render(renderer);
        gun->render(renderer, rightX, rightY);
        healthbar->render(renderer);

        for (auto it = bullets.begin(); it != bullets.end(); ) {
            if (!it->update(deltaTime)) {
                it = bullets.erase(it);
            }
            else {
                it->render(renderer);
                ++it;
            }
        }

        SDL_RenderPresent(renderer);
    }

    delete player;
    delete gun;

    if (bgTexture != nullptr) { SDL_DestroyTexture(bgTexture); }
    if (playerTexture != nullptr) { SDL_DestroyTexture(playerTexture); }
    if (gunTexture != nullptr) { SDL_DestroyTexture(gunTexture); }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    return 0;
}