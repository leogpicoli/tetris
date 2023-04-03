#include <iostream>
#include <SDL2/SDL.h>
#include <array>
#include <color.hpp>
#include <pos.hpp>
#include <tetrisMap.hpp>
#include <tetrimino.hpp>
#include <client.hpp>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <unistd.h>
#include <menu.hpp>
#include <menuRoom.hpp>
#include <mutex>
#include "results.hpp"

using namespace std;

bool windowOpen;
mutex mx;

void render(TetrisMap *tetrisMap, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    tetrisMap->draw(renderer);
    SDL_RenderPresent(renderer);
}

void physics(TetrisMap *tetrisMap)
{
    tetrisMap->tick();
}

void input(TetrisMap *tetrisMap)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            windowOpen = false;
        if (event.key.state == SDL_PRESSED)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_z:
                tetrisMap->tetriminoAction(ROTATE_LEFT);
                break;
            case SDLK_x:
                tetrisMap->tetriminoAction(ROTATE_RIGHT);
                break;
            case SDLK_c:
                tetrisMap->changeHold();
                break;
            case SDLK_LEFT:
                tetrisMap->tetriminoAction(MOVE_LEFT);
                break;
            case SDLK_RIGHT:
                tetrisMap->tetriminoAction(MOVE_RIGHT);
                break;
            case SDLK_UP:
                tetrisMap->tetriminoAction(ROTATE_RIGHT);
                break;
            case SDLK_DOWN:
                tetrisMap->tetriminoAction(MOVE_DOWN);
                break;
            case SDLK_SPACE:
                tetrisMap->tetriminoAction(DROP);
                break;
            }
        }
    }
}

// Responsable for getting enemy map from server and to send our map to the server
void handleClient(Client *client, TetrisMap *tetrisMap)
{
    bool multiplayerGameOver = false;
    int mapSize = MATRIX_WIDTH * MATRIX_HEIGHT;
    while (!tetrisMap->isGameOver() && !multiplayerGameOver && windowOpen)
    {
        usleep(50000); // Sleep for 50ms

        /* Send map */
        char code = CODE_PLAYER_MAP;
        char my_map[mapSize];
        tetrisMap->getMap(my_map);
        client->send(&code, 1);
        client->send(my_map, mapSize);

        /* Send buffer lines*/
        int bufferLines = tetrisMap->getBufferLines();
        if (bufferLines < 0)
        {
            char buf = (char)(-bufferLines);
            code = CODE_PLAYER_LINES;
            client->send(&code, 1);
            client->send(&buf, 1);
            tetrisMap->resetBufferLines();
        }

        client->recv(&code, 1);
        if (code == CODE_PLAYER_MAP)
            client->recv(tetrisMap->enemyMap(), mapSize);
        else if (code == CODE_PLAYER_LINES)
        {
            char buf = 0;
            client->recv(&buf, 1);
            bufferLines = (int)buf;
            tetrisMap->addBufferLines(bufferLines);
        }
        else if (code == CODE_GAME_OVER)
            multiplayerGameOver = true;
    }

    if (multiplayerGameOver)
        cout << "You won, congratulations!" << endl;
    else if (tetrisMap->isGameOver())
    {
        char code = CODE_PLAYER_DEAD;
        client->send(&code, 1);
        cout << "You lost, keep trying and one day you will be a Tetris Pro!" << endl;
    }

    // While to keep watchin others
    while (!multiplayerGameOver && windowOpen)
    {
        usleep(50000); // Sleep for 50ms
        char code = CODE_PLAYER_MAP;
        client->recv(&code, 1);
        if (code == CODE_PLAYER_MAP)
            client->recv(tetrisMap->enemyMap(), mapSize);
        else if (code == CODE_GAME_OVER)
            multiplayerGameOver = true;
    }

    windowOpen = false;
}

void runTetrisMultiplayer(Client *client)
{
    TetrisMap *tetrisMap = new TetrisMap(true);
    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH_MULTIPLAYER, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Tetris");
    windowOpen = true;

    thread t_client(handleClient, client, tetrisMap);

    while (windowOpen)
    {
        input(tetrisMap);
        physics(tetrisMap);
        render(tetrisMap, renderer);
    }

    t_client.join();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    int score, linesCleared, level;
    tetrisMap->getGameStatus(&score, &linesCleared, &level);
    Results results(score, linesCleared, !tetrisMap->isGameOver());
}

void runTetrisSingleplayer()
{
    TetrisMap *tetrisMap = new TetrisMap(false);
    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Tetris");
    windowOpen = true;

    while (!tetrisMap->isGameOver() && windowOpen)
    {
        input(tetrisMap);
        physics(tetrisMap);
        render(tetrisMap, renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    int score, linesCleared, level;
    tetrisMap->getGameStatus(&score, &linesCleared, &level);
    Results results(score, linesCleared, !tetrisMap->isGameOver());
}

void handleClientRoom(MenuRoom *menuRoom, Client *client)
{
    char code;
    client->recv(&code, 1);
    if (code != CODE_ROOM_UNAVAILABLE)
    {
        while (code == CODE_ROOM_SIZE)
        {
            char frac[3];
            client->recv(frac, 3);
            string playersInRoom(frac);
            menuRoom->setPlayers(frac);
            client->recv(&code, 1);
        }
        if (code == CODE_START_GAME)
        {
            menuRoom->setStatus(MENU_ROOM_STARTING);
        }
    }
    else
        menuRoom->setStatus(MENU_ROOM_UNAVAILABLE);
}

int main(int argc, char *argv[])
{
    srand((unsigned)time(NULL));
    const char *hostname = argv[1];
    int option = 0;
    while (option != 2)
    {
        Menu menu;

        if (!menu.hasChosen())
            break;

        option = menu.getChoice();

        /* Tetris singleplayer mode! */
        if (option == 0)
            runTetrisSingleplayer();

        /* Tetris multiplayer option */
        if (option == 1)
        {
            Client *client = new Client(hostname, SERVER_PORT);

            if (client->is_connected())
            {
                MenuRoom *menuRoom = new MenuRoom();

                thread t1(handleClientRoom, menuRoom, client);
                t1.detach();
                menuRoom->runMainLoop();

                if (menuRoom->getStatus() == MENU_ROOM_STARTING)
                {
                    runTetrisMultiplayer(client);
                    client->disconnect();
                }
                else
                {
                    char code = CODE_PLAYER_DISCONNECT;
                    client->send(&code, 1);
                    // client->disconnect();
                }
            }
        }
    }

    return 0;
}