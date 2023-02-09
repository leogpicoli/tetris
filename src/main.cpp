#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <color.hpp>
#include <pos.hpp>

using namespace std;

const int WIDTH = 800, HEIGHT = 600;

// class Mino {
//     private:
//         Color color;
// };

// class Tetromino {
//     private:

// };

int main(int argc, char *argv[])
{
    Pos p(10, 11);
    Pos p2(3, 2);
    Color red = Color(255, 0, 0, 0);
    cout << "(" << p2.row() << ", " << p2.col() << ")" << endl;
    // SDL_Window* window = NULL;

    // SDL_Init(SDL_INIT_VIDEO);
    // window = SDL_CreateWindow(
    //     "An SDL Window",
    //     SDL_WINDOWPOS_UNDEFINED,
    //     SDL_WINDOWPOS_UNDEFINED,
    //     640,
    //     480,
    //     SDL_WINDOW_OPENGL
    // );

    // if (window == NULL) {
    //     cout << "deu pau" << endl;
    //     return 1;
    // }

    // SDL_Renderer *renderer = NULL;
    // renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // SDL_Rect rect;

    // rect.x = 200;
    // rect.y = 200;
    // rect.h = 50;
    // rect.w = 50;

    // // Infinite loop for application
    // bool gameIsRunning = true;
    // while (gameIsRunning) {
    //     SDL_Event event;
    //     int x, y;
    //     SDL_GetMouseState(&x, &y);

    //     while(SDL_PollEvent(&event)) {
    //         if (event.type == SDL_QUIT) {
    //             gameIsRunning = false;
    //         }
    //     }

    //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    //     SDL_RenderClear(renderer);

    //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    //     rect.x = x - 25;
    //     rect.y = y - 25;
    //     SDL_RenderDrawRect(renderer, &rect);

    //     SDL_RenderPresent(renderer);
    // }

    // SDL_DestroyWindow(window);

    // SDL_Quit();

    return 0;
}