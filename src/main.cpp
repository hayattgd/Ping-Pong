#include <SDL2/SDL.h>
#include <cmath>
#include <stdio.h>

#include "main.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 480

#define TARGET_FPS 60

void BallInitialize() {
    Ball[0] = 390; // x position
    Ball[1] = 230; // y position
    Ball[2] = 0; // angle
    Ball[3] = 3.0; //speed
}

bool isYOutsideScreen(int y, int height) {
    return y < 0 || y + height > WINDOW_HEIGHT;
}

int isXOutsideScreen(int x, int width) {
    return x < 0 || x + width > WINDOW_WIDTH;
}

int main(int argc, char *argv[])
{
    bool running = true;

    printf("Initializing SDL2...\n");
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize SDL %s\n", SDL_GetError());
        running = false;
    }

    SDL_Window *window = SDL_CreateWindow("Ping pong",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_SHOWN);
    if(!window) {
        printf("Failed to create window %s\n", SDL_GetError());
        running = false;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer) {
        printf("Failed to create renderer %s\n", SDL_GetError());
        running = false;
    }

    if(running)
        printf("Initialize completed!\n");

    SDL_Event ev;

    Paddle Player;
    Paddle Bot;

    int desiredDelta = 1000 / TARGET_FPS;

    BallInitialize();

    //Main Game loop
    while(running) {
        int starttick = SDL_GetTicks();

        SDL_SetRenderDrawColor(renderer, 32, 32, 54, 255);
        SDL_RenderClear(renderer);

        //Inputs
        while(SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) {
                running = false;
            }
            else if(ev.key.repeat == 0) {
                if(ev.type == SDL_KEYDOWN) {
                    if(ev.key.keysym.sym == SDLK_UP) {
                        Player.velocity += 4;
                    }
                    else if(ev.key.keysym.sym == SDLK_DOWN) {
                        Player.velocity -= 4;
                    }
                }
                else if(ev.type == SDL_KEYUP) {
                    if(ev.key.keysym.sym == SDLK_UP) {
                        Player.velocity -= 4;
                    }
                    else if(ev.key.keysym.sym == SDLK_DOWN) {
                        Player.velocity += 4;
                    }
                }
            }
        }

        ////Ball and Player/Bot
        //Player
        SDL_Rect *player = new SDL_Rect{50, Player.y, 10, 100};
        int Moving_to;
        Moving_to = Player.y - Player.velocity;
        
        if(!isYOutsideScreen(Moving_to, 100))
            Player.y -= Player.velocity;
        

        //Bot
        SDL_Rect *bot = new SDL_Rect{750, Bot.y, 10, 100};

        if (Ball[1] < Bot.y + 20) {
            Bot.velocity = 4;
        } else if (Ball[1] > Bot.y + 80) {
            Bot.velocity = -4;
        } else {
            Bot.velocity = 0;
        }
        Moving_to = Bot.y - Bot.velocity;
        
        if(!isYOutsideScreen(Moving_to, 100))
            Bot.y -= Bot.velocity;
        
        //Ball
        SDL_Rect *ball = new SDL_Rect{int(Ball[0]), int(Ball[1]), 10, 10};

        // Check collision with top and bottom of the window
        if (isYOutsideScreen(Ball[1], 10)) { // 10 is the ball's height
            Ball[2] *= -1;
        }

        if (isXOutsideScreen(Ball[0], 10)) {
            BallInitialize(); // rip Ball
        }

        if (SDL_HasIntersection(ball, player)) {
            Ball[3] += 0.3;
            Ball[2] += 180 - (((Player.y + 50) - Ball[1]) * 0.4f);
        }

        if (SDL_HasIntersection(ball, bot)) {
            Ball[3] += 0.3;
            Ball[2] += 180 - (((Bot.y + 50) - Ball[1]) * 0.4f);
        }

        Ball[0] += Ball[3] * SDL_cosf(Ball[2] * (M_PI / 180));
        Ball[1] += Ball[3] * SDL_sinf(Ball[2] * (M_PI / 180));

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        //Player
        SDL_RenderFillRect(renderer, player);

        //Bot
        SDL_RenderFillRect(renderer, bot);

        //Ball
        SDL_RenderFillRect(renderer, ball);

        SDL_RenderPresent(renderer);
        
        int delta = SDL_GetTicks() - starttick;
        if(delta < desiredDelta) {
            SDL_Delay(desiredDelta - delta);
        }
    }

    printf("Finishing Ping pong...\n");

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Finished!\n%s\n", SDL_GetError());
    return 0;
}
