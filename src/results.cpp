#include <results.hpp>
#include <globals.hpp>
#include <iostream>

#define MAX_CHOICES 3

using namespace std;

Results::Results(int score, int lines_cleared, bool win)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH_MENU, WINDOW_HEIGHT_MENU, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Tetris");

    menuShowing = true;

    textSmaller = new Text(18);
    textSmall = new Text(24);
    textBig = new Text(60);

    bg = new Background(renderer);

    while (menuShowing)
    {
        input();
        render(score, lines_cleared, win);
        bg->tick();
    }

    close();
}

void Results::input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            menuShowing = false;
        if (event.key.state == SDL_PRESSED)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_RETURN:
            {
                menuShowing = false;
            }
            break;
            }
        }
    }
}

void Results::render(int score, int linesCleared, bool win)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    string results = "Results of";
    string tetris = "TETRIS";
    string oldskool = "OLDSKOOL";
    string win_lose = win ? "Congratulations, you won!" : "Sorry, you lost!";
    string str_score = "Your score: " + to_string(score);
    string str_lines = "Lines cleared: " + to_string(linesCleared);
    string enter = "Press enter to continue";
    string madeWithLove = "Made with <3 by Leo and Magro";

    bg->render();

    textSmall->drawCenter(0, 50, WINDOW_WIDTH_MENU, 50, results, renderer);
    textBig->drawCenter(0, 100, WINDOW_WIDTH_MENU, 100, tetris, renderer);
    textBig->drawCenter(0, 160, WINDOW_WIDTH_MENU, 160, oldskool, renderer);

    textSmall->drawCenter(0, 300, WINDOW_WIDTH_MENU, 300, win_lose, renderer);
    textSmall->drawCenter(0, 350, WINDOW_WIDTH_MENU, 350, str_score, renderer);
    textSmall->drawCenter(0, 400, WINDOW_WIDTH_MENU, 400, str_lines, renderer);

    textSmaller->drawCenter(0, 520, WINDOW_WIDTH_MENU, 520, enter, renderer);
    textSmaller->drawCenter(0, 550, WINDOW_WIDTH_MENU, 550, madeWithLove, renderer);

    SDL_RenderPresent(renderer);
}

void Results::close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}