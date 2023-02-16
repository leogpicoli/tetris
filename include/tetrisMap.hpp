#ifndef TETRIS_MAP_HPP
#define TETRIS_MAP_HPP

#include <pos.hpp>
#include <tetrimino.hpp>
#include <SDL2/SDL.h>
#include <globals.hpp>
#include <vector>
#include <mino.hpp>

enum TetrisPhase {
    GENERATION_PHASE,
    FALLING_PHASE,
    LOCK_PHASE,
    PATTERN_PHASE,
    ANIMATION_PHASE,
    ELIMINATE_PHASE,
};

class TetrisMap
{
public:
    TetrisMap();
    Mino at(Pos pos);
    Mino at(int row, int col);

    void set(int row, int col, Mino m);
    void set(Pos pos, Mino m);
    void draw(SDL_Renderer *renderer);
    void tetriminoAction(TetriminoAction action);

    // Tick
    void tick();

    void lockTetrimino();

    void generateNextTetrimino();

    void copyRow(int row_source, int row_destiny);
private:
    // Matrix Height + 2 (Skyline) + 2 (Top wall) + 2 (Bottom wall)
    // Matrix Width + 2 (Left wall) + 2 (Right wall)
    // char can be: O, I, T, L, J, S, Z, W (for wall)
    Mino m_matrix[MATRIX_HEIGHT + 2 + 2 + 2][MATRIX_WIDTH + 2 + 2];
    // Tetrim
    Tetrimino *tetrimino;

    int fall_speed = 500; // milliseconds
    int t_down_time = fall_speed;

    TetrisPhase phase;

    vector<int> rowsDestroyed;
    vector<char> virtualBag = {'I', 'O', 'T', 'L', 'J', 'Z', 'S'};
    vector<Tetrimino*> tetriminoQueue;

    // Undoes tetriminoAction if it is colliding with something, returns true if it was validated.
    bool validateTetriminoAction();
    void initTetriminoQueue();
    bool canFall();
    void drawGhostMinos(SDL_Renderer *renderer);
    
    int lockDownTimer;
};

#endif
