#include <iostream>
#include <SDL2/SDL.h>
#include <array>
#include <color.hpp>
#include <pos.hpp>
#include <tetrisMap.hpp>
#include <tetrimino.hpp>
#include <ctime>
#include <cstdlib>

using namespace std;


bool running;

SDL_Renderer *renderer;
SDL_Window *window;

TetrisMap *tetrisMap;

void render() {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    tetrisMap->draw(renderer);
    SDL_RenderPresent(renderer);
}

void physics() {
    tetrisMap->tick();
}

void input(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT) running=false;
        if (event.key.state == SDL_PRESSED) {
            switch (event.key.keysym.sym) {
                case SDLK_z: tetrisMap->tetriminoAction(ROTATE_LEFT);
                break;
                case SDLK_x: tetrisMap->tetriminoAction(ROTATE_RIGHT);
                break;
                case SDLK_LEFT: tetrisMap->tetriminoAction(MOVE_LEFT);
                break;
                case SDLK_RIGHT: tetrisMap->tetriminoAction(MOVE_RIGHT);
                break;
                case SDLK_UP: tetrisMap->tetriminoAction(ROTATE_RIGHT);
                break;
                case SDLK_DOWN: tetrisMap->tetriminoAction(MOVE_DOWN);
                break;
                case SDLK_SPACE: tetrisMap->tetriminoAction(DROP);
                break;
            }
        }
    }
}

int main(int argc, char *argv[]){
    srand((unsigned) time(NULL));

    tetrisMap = new TetrisMap();
    running = true;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Tetris");

    while(running){
        input();
        physics();
        render();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}