#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include "Player.h"
#include "Gun.h"
#include "Healthbar.h"
#include "Bullet.h"
#include "Balloon.h"
#include "Plane.h"

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

    Healthbar* healthbar = new Healthbar(player, healthbarTexture);

    SDL_Surface* bgSurface = IMG_Load("Media/background.png");
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    SDL_Surface* balloonSurface = IMG_Load("Media/balloon-sheet.png");
    SDL_Texture* balloonTexture = SDL_CreateTextureFromSurface(renderer, balloonSurface);
    SDL_FreeSurface(balloonSurface);

    SDL_Surface* planeSurface = IMG_Load("Media/plane-sheet.png");
    SDL_Texture* planeTexture = SDL_CreateTextureFromSurface(renderer, planeSurface);
    SDL_FreeSurface(planeSurface);

    std::vector<Bullet> bullets;
    std::vector<Balloon> balloons;
    std::vector<Plane> planes;

    bool isRunning{ true };
    SDL_Event event;

    Uint32 lastTime = SDL_GetTicks();
    Uint32 balloonSpawnTimer = 0;
    const Uint32 BALLOON_SPAWN_COOLDOWN = 3000;
    Uint32 planeSpawnTimer = 0;
    const Uint32 PLANE_SPAWN_COOLDOWN = 5000;
    Uint32 shootTimer = 0;
    const Uint32 SHOOT_RATE = 700;

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

            if (r2Value > 1000 && shootTimer >= SHOOT_RATE) {
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

                bullets.push_back(Bullet(startX, startY, gun->angle, 0, true, bulletTexture));

                shootTimer = 0;
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        Uint32 frameTime = currentTime - lastTime;
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        balloonSpawnTimer += frameTime;
        if (balloonSpawnTimer >= BALLOON_SPAWN_COOLDOWN) {
            if (balloons.size() < 2) { balloons.push_back(Balloon(0.0f, 20.0f, 0, balloonTexture)); }
            balloonSpawnTimer -= BALLOON_SPAWN_COOLDOWN;
        }

        planeSpawnTimer += frameTime;
        if (planeSpawnTimer >= PLANE_SPAWN_COOLDOWN) {
            if (planes.size() < 2) { planes.push_back(Plane(320.0f, 240.0f, 0, player, planeTexture)); }
            planeSpawnTimer -= PLANE_SPAWN_COOLDOWN;
        }

        if (shootTimer < SHOOT_RATE) { shootTimer += frameTime; }

        player->update(deltaTime);
        gun->update();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (bgTexture != nullptr) {
            SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
        }

        player->render(renderer);
        gun->render(renderer, rightX, rightY);

        for (auto it = bullets.begin(); it != bullets.end(); ) {
            if (!it->update(deltaTime)) {
                it = bullets.erase(it);
            }
            else {
                it->render(renderer);
                ++it;
            }
        }

        for (auto it = balloons.begin(); it != balloons.end(); ) {
            if (!it->update(deltaTime)) {
                it = balloons.erase(it);
            }
            else {
                it->render(renderer);
                ++it;
            }
        }

        for (auto it = planes.begin(); it != planes.end(); ) {
            if (!it->update(deltaTime, frameTime)) {
                it = planes.erase(it);
            }
            else {
                it->render(renderer);
                ++it;
            }
        }

        for (auto bIt = bullets.begin(); bIt != bullets.end(); ) {
            float bulletCenterX = bIt->x + (bIt->frameWidth / 2);
            float bulletCenterY = bIt->y + (bIt->frameHeight / 2);
            bool bulletHit = false;

            if (bIt->isFriendly) {
                for (auto balloonIt = balloons.begin(); balloonIt != balloons.end(); ++balloonIt) {
                    float balloonCenterX = balloonIt->x + (balloonIt->frameWidth / 2);
                    float balloonCenterY = balloonIt->y + (balloonIt->frameHeight / 2);
                    float distance = std::sqrt((balloonCenterX - bulletCenterX) * (balloonCenterX - bulletCenterX) + (balloonCenterY - bulletCenterY) * (balloonCenterY - bulletCenterY));
                    if (distance < 25) {
                        balloonIt->health -= bIt->damage;
                        bulletHit = true;
                        break;
                    }
                }
            }
            else {
                float playerCenterX = player->x + (player->width / 2);
                float playerCenterY = player->y + (player->height / 2);
                float distance = std::sqrt((playerCenterX - bulletCenterX) * (playerCenterX - bulletCenterX) + (playerCenterY - bulletCenterY) * (playerCenterY - bulletCenterY));
                if (distance < 25) {
                    player->health -= bIt->damage;
                    if (player->health <= 0) {
                        isRunning = false;
                    }
                    bulletHit = true;
                }
            }

            if (bulletHit) {
                bIt = bullets.erase(bIt);
            }
            else {
                ++bIt;
            }
        }

        for (auto pIt = planes.begin(); pIt != planes.end(); ++pIt) {
            if (pIt->state == 2 && pIt->shootTimer >= pIt->shootCooldown) {
                float planeCenterX = pIt->x + (pIt->frameWidth / 2.0f);
                float planeCenterY = pIt->y + (pIt->frameHeight / 2.0f);
                
                const float PI = 3.14159265f;
                float radians = pIt->degrees * (PI / 180.0f);

                float planeSteps = 25.0f;
                float dx = std::cos(radians) * planeSteps;
                float dy = std::sin(radians) * planeSteps;

                float startX = planeCenterX + dx;
                float startY = planeCenterY + dy;

                bullets.push_back(Bullet(startX, startY, pIt->degrees, 0, false, bulletTexture));
                pIt->shootTimer = 0;
            }
        }

        healthbar->render(renderer);

        SDL_RenderPresent(renderer);
    }

    delete player;
    delete gun;

    if (bgTexture != nullptr) { SDL_DestroyTexture(bgTexture); }
    if (playerTexture != nullptr) { SDL_DestroyTexture(playerTexture); }
    if (gunTexture != nullptr) { SDL_DestroyTexture(gunTexture); }
    if (balloonTexture != nullptr) { SDL_DestroyTexture(balloonTexture); }
    if (planeTexture != nullptr) { SDL_DestroyTexture(planeTexture); }
    if (bulletTexture != nullptr) { SDL_DestroyTexture(bulletTexture); }
    if (healthbarTexture != nullptr) { SDL_DestroyTexture(healthbarTexture); }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    return 0;
}