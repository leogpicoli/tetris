#ifndef TETRIS_TEXT_HPP
#define TETRIS_TEXT_HPP

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <string>
using namespace std;

class Text
{
public:
    Text();
    ~Text();
    void draw(int x, int y, string s, SDL_Renderer *renderer);
    void drawCentered(int startX, int startY, int endX, int endY, string s, SDL_Renderer *renderer);

private:
    TTF_Font *font;
};

#endif
