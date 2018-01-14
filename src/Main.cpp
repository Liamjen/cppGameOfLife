
#define SDL_MAIN_HANDLED

#include <iostream>
#include <string>
#include "SDL2/SDL.h"
#include "GameOfLife.hpp"

int main(int argc, char* args[])
{
    int SCREEN_WIDTH, SCREEN_HEIGHT, renderScale;

    //Get the last 3 arguments to set screen size and render scale
    bool defaultSize = false; 
    for(int i = argc - 3; i < argc; i++)
    {
        if(i <= 0)
        {
            defaultSize = true;
            break;
        }
        else if(!std::stoi(args[i]))
        {
            defaultSize = true;
            break;
        }
    }

    if(defaultSize)
    {
        SCREEN_WIDTH = 200;
        SCREEN_HEIGHT = 200;
        renderScale = 4;
    }
    else
    {
        SCREEN_WIDTH = std::stoi(args[argc - 3]);    
        SCREEN_HEIGHT = std::stoi(args[argc - 2]);    
        renderScale = std::stoi(args[argc - 1]);    
    }

    GameOfLife* gameOfLife = new GameOfLife(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return 1;
   
    SDL_Window *window = SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * renderScale, SCREEN_HEIGHT * renderScale, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event event;
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH * renderScale, SCREEN_HEIGHT * renderScale);
    SDL_RenderSetScale(renderer, renderScale, renderScale);
    bool running = true;

    bool paused = false;
    //Delay in ms to next generation
    int delay = 10;

    //Main SDL loop
    while(running)
    {
        SDL_Delay(delay);
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
                    case SDLK_p:
                        paused = !paused;
                        break;
                    case SDLK_RIGHT:
                        gameOfLife->nextGeneration();
                        break;
                    case SDLK_UP:
                        delay += 10;
                        break;
                    case SDLK_DOWN:
                        delay -= 10;
                        if(delay < 0)
                            delay = 0;
                        break;
                    }
                }
            }

        if(!paused)
            gameOfLife->nextGeneration();
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
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

