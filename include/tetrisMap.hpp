#ifndef TETRIS_MAP_HPP
#define TETRIS_MAP_HPP

#include <pos.hpp>
#include <tetrimino.hpp>
#include <SDL2/SDL.h>
#include <globals.hpp>

class TetrisMap
{
public:
    TetrisMap();
    char at(Pos pos);
    char at(int row, int col);
    void draw(SDL_Renderer *renderer);
    void tetriminoAction(TetriminoAction action);

    // Automatically goes down
    void goDown(TetriminoAction mov);
    // Draw dead tetrimino
    void drawDeadTetrimino(SDL_Renderer *renderer);

private:
    // Matrix Height + 2 (Skyline) + 2 (Top wall) + 2 (Bottom wall)
    // Matrix Width + 2 (Left wall) + 2 (Right wall)
    // char can be: O, I, T, L, J, S, Z, W (for wall)
    char m_matrix[MATRIX_HEIGHT + 2 + 2 + 2][MATRIX_WIDTH + 2 + 2];
    // Tetrim
    Tetrimino *tetrimino;

    // Undoes tetriminoAction if it is colliding with something, returns true if it was validated.
    bool validateTetriminoAction();
};

#endif