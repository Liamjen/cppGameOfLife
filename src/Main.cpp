
#define SDL_MAIN_HANDLED

#include <iostream>
#include "SDL2/SDL.h"
#include "GameOfLife.hpp"

int main(int argc, char* args[])
{
    //TODO: Temporary size variables
    int SCREEN_WIDTH = 1000;
    int SCREEN_HEIGHT = 1000;
    int renderScale = 1;

    GameOfLife* gameOfLife = new GameOfLife(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return 1;
   
    SDL_Window *window = SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |SDL_RENDERER_PRESENTVSYNC);
    SDL_Event event;
    SDL_RenderSetScale(renderer, renderScale, renderScale);
    bool running = true;

    //Main SDL loop
    while(running)
    {
        while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                    running = false;
                else if (event.type == SDL_KEYDOWN)
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    }
                }
            }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        gameOfLife->nextGeneration();
        Cell** grid = gameOfLife->getGrid();
        for(int i = 0; i < SCREEN_WIDTH; i++)
            for(int j = 0; j < SCREEN_HEIGHT; j++)
            {
                if(grid[i][j].alive)
                {
                    SDL_RenderDrawPoint(renderer, i, j);
                }
            }

        SDL_RenderPresent(renderer);
    }
    
    //End program
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
    
}

