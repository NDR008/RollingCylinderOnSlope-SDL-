// quick implementation of rolling cylinder on slopped "plane"
// Nadir Syed 25/02/2021

#include <iostream>
#include <algorithm>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Clock
{
    uint32_t last_tick_time = 0;
    uint32_t tick_time = 0;
    uint32_t delta = 0;
    float dT = 0.000000001f;

    void tick()
    {
        tick_time = SDL_GetTicks();
        delta = tick_time - last_tick_time;
        dT = (float)delta/1000.0f;
        last_tick_time = tick_time;
    }
};

const float initial_x = 700; // 1 pixel = 1 meter.
const float initial_y = 100;

struct Cylinder
{
    float r = 20;
    float velocity=0;
    SDL_Rect texture;
    float position=0;
    float real_x = initial_x;
    float real_y = initial_y;
};

const float g = 9.81f; 
const float cos_sin45 = 0.70710678118f;

SDL_Surface* surface; 
SDL_Texture* texture;

const float offset = sqrt(200);
// 20^2 = A^2 + A^2
// 400 = 2 A^2
// 200 = A^2


int main(int argv, char** args)
{
    Cylinder cylinder;
    cylinder.texture.x = initial_x;
    cylinder.texture.y = initial_y;
    cylinder.texture.h = 40;
    cylinder.texture.w = 40;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Rolling down", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    bool isRunning = true;
    SDL_Event event;
    surface = IMG_Load("ball.png"); // load the image
    texture = SDL_CreateTextureFromSurface(renderer, surface); // not sure what this is but... converts image to texture?

    Clock mainClock;

    while (isRunning) {    
        const Uint8 *keyState = SDL_GetKeyboardState(NULL);
        while( SDL_PollEvent( &event ) ){
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }
        if (keyState[SDL_SCANCODE_SPACE]){
            cylinder.position = 0;
            cylinder.velocity = 0;
            cylinder.real_x = initial_x;
            cylinder.real_y = initial_y;
        }
        else{
        }
        mainClock.tick();
        // will put main physics here to recalc new x and new y position of ball....
        float dt = mainClock.dT;
        
        float current_velocity = cylinder.velocity;
        cylinder.velocity += 2 * g * cos_sin45 * dt / 3;
        cylinder.position += (current_velocity + cylinder.velocity) / 2 * dt;
        cylinder.real_x = initial_x - cylinder.position* cos_sin45;
        cylinder.real_y = initial_y + cylinder.position* cos_sin45;

        cylinder.texture.x = cylinder.real_x;
        cylinder.texture.y = cylinder.real_y;

        std::cout << cylinder.position << "\t" << cylinder.texture.x << "\t" << cylinder.texture.y << std::endl;
        std::cout << cylinder.position << "\t" << cylinder.real_x << "\t" << cylinder.real_y << std::endl;
        std::cout << "";

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // i believe alpha is the last term
        SDL_RenderClear(renderer);
        // change the draw color for the line
        // change the blending mode for the line
        SDL_SetRenderDrawColor(renderer, 150, 0, 0, 255);
        //SDL_RenderFillRect(renderer, &cylinderTexture);
        SDL_RenderDrawLine(renderer, initial_x+40+offset-500, initial_y+offset+500, initial_x+40+offset, initial_y+offset);
        SDL_RenderCopy(renderer, texture, NULL, &cylinder.texture);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
