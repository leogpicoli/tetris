#ifndef TETRIS_MENU_ROOM_HPP
#define TETRIS_MENU_ROOM_HPP

#include <SDL2/SDL.h>
#include <text.hpp>
#include <background.hpp>

#define MENU_ROOM_UNAVAILABLE 0
#define MENU_ROOM_WAITING 1
#define MENU_ROOM_STARTING 2

using namespace std;

class MenuRoom
{
public:
    MenuRoom();
    void input();
    void render();
    void close();
    void runMainLoop();
    void setStatus(int new_status);
    void setPlayers(string newPlayers);

private:
    SDL_Renderer *renderer;
    SDL_Window *window;
    Background *bg;
    Text *textSmall, *textSmaller, *textBig;

    int status; // 0 for room unav, 1 for waiting players, 2 for get ready message
    string playersInTheRoom;
    bool menuShowing;
};

#endif