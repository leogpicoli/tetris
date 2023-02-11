#include <iostream>
#include <SDL2/SDL.h>
#include <array>
#include <color.hpp>
#include <pos.hpp>
#include <tetrisMap.hpp>
#include <tetrimino.hpp>

using namespace std;

const int WIDTH = 360, HEIGHT = 720;
bool running;

SDL_Renderer *renderer;
SDL_Window *window;

TetrisMap tetrisMap;

void render() {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    tetrisMap.draw(renderer);
    tetrisMap.goDown(MOVE_DOWN);
    tetrisMap.drawDeadTetrimino(renderer);
    SDL_RenderPresent(renderer);
}

void input(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT) running=false;
        if (event.key.state == SDL_PRESSED) {
            switch (event.key.keysym.sym) {
                case SDLK_z: tetrisMap.tetriminoAction(ROTATE_LEFT);
                break;
                case SDLK_x: tetrisMap.tetriminoAction(ROTATE_RIGHT);
                break;
                case SDLK_LEFT: tetrisMap.tetriminoAction(MOVE_LEFT);
                break;
                case SDLK_RIGHT: tetrisMap.tetriminoAction(MOVE_RIGHT);
                break;
                case SDLK_UP: tetrisMap.tetriminoAction(MOVE_UP);
                break;
                case SDLK_DOWN: tetrisMap.tetriminoAction(MOVE_DOWN);
                break;
            }
        }
    }
}

int main(int argc, char *argv[]){
    running = true;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Tetris");

    while(running){
        input();
        render();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}