#include <tetrisMap.hpp>
#include <pos.hpp>
#include <color.hpp>
#include <SDL2/SDL.h>
#include <iostream>
#include <tetrimino.hpp>
#include <globals.hpp>

TetrisMap::TetrisMap()
{
    for (int row = 0; row < MATRIX_HEIGHT + 6; row++)
    {
        for (int col = 0; col < MATRIX_WIDTH + 2 + 2; col++)
        {
            /*
                Example of representation of what we are creating here
                WWWWWWWWWWWWWW
                WWWWWWWWWWWWWW
                WW  SKYLINE WW
                WW  SKYLINE WW
                WW  IIII    WW
                WW          WW
                WW          WW
                WW SST   L  WW
                WWSSTTTLLL  WW
                WWWWWWWWWWWWWW
                WWWWWWWWWWWWWW
                This representation is preferred because, like this, we can access positions in the matrix
                when doing rotation and shifts in the tetrominoes without the need to worry with SEG FAULT
                Empty spaces are filled with 0, and the walls are filled as W in the matrix
            */
            m_matrix[row][col].value() = 0;

            if (col < 2 || col >= MATRIX_WIDTH + 2 || row < 2 || row >= MATRIX_HEIGHT + 4)
            {
                m_matrix[row][col].value() = 'W';
            }
        }
    }

    phase = GENERATION_PHASE;

    srand((unsigned)time(NULL));

    initTetriminoQueue();
}

Mino TetrisMap::at(int row, int col)
{
    // Since tetrominoes start at position -2 and -1 (positions representing the skyline)
    // And since we have 2 layers of walls above the skyline, we need to add 4 positions to the row
    // So, row -2 is actually represented as row 2 in the matrix, row 0 is actually row 4 in the matrix, etc...
    // Since we have 2 layers of wall in the left side we have to add 2 lines to the column;
    return m_matrix[row + 4][col + 2];
}

void TetrisMap::set(int row, int col, Mino m)
{
    m_matrix[row + 4][col + 2] = m;
}

void TetrisMap::set(Pos p, Mino m)
{
    set(p.row(), p.col(), m);
}

Mino TetrisMap::at(Pos p)
{
    return at(p.row(), p.col());
}

void TetrisMap::draw(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);

    for (auto row = 0; row < MATRIX_HEIGHT; row++)
    {
        SDL_RenderDrawLine(renderer, 0, row * TILE_SIZE, WINDOW_WIDTH, row * TILE_SIZE);
    }

    for (auto col = 0; col < MATRIX_WIDTH; col++)
    {
        SDL_RenderDrawLine(renderer, col * TILE_SIZE, 0, col * TILE_SIZE, WINDOW_HEIGHT);
    }

    tetrimino->draw(renderer);

    for (auto row = 0; row < MATRIX_HEIGHT; row++)
    {
        for (auto col = 0; col < MATRIX_WIDTH; col++)
        {   
            Mino m = at(row, col);
            m.draw(renderer, row, col);
        }
    }
}

void TetrisMap::tetriminoAction(TetriminoAction action)
{
    // Move action
    if (action == MOVE_DOWN || action == MOVE_RIGHT || action == MOVE_LEFT || action == MOVE_UP)
    {
        tetrimino->moveAction(action);
        validateTetriminoAction();
    }
    else
    { // Rotate action
        for (int i = 1; i <= 5; i++)
        { // Test each rotation point
            tetrimino->rotateAction(action, i);

            // If it is a valid rotation, stop the for that tests each rotation point.
            if (validateTetriminoAction())
                break;
        }
    }
}

// Put char representing dead tetrimino to the matrix
void TetrisMap::lockTetrimino()
{
    Pos t_pos;
    array<Pos, 4> minos = tetrimino->getMinos();
    for (int i = 0; i < 4; i++)
    {
        t_pos = minos[i] + tetrimino->pos();
        Mino m(tetrimino->name());
        set(t_pos, m);
    }
    tetrimino->setDead();
}

void TetrisMap::generateNextTetrimino()
{
    tetrimino = tetriminoQueue.at(0);
    tetriminoQueue.erase(tetriminoQueue.begin());

    int ti = rand() % virtualBag.size();
    char name = virtualBag.at(ti);
    virtualBag.erase(virtualBag.begin() + ti);

    if (virtualBag.empty())
    {
        virtualBag = VIRTUAL_BAG;
    }

    tetriminoQueue.push_back(Tetrimino::generateTetriminoFrom(name));
}

// Engine flow
void TetrisMap::tick()
{
    // Make tetrimino falls
    if (phase == GENERATION_PHASE)
    {
        generateNextTetrimino();
        phase = FALLING_PHASE;
        t_down_time = SDL_GetTicks();
    }
    if (phase == FALLING_PHASE)
    {
        if (canFall())
        {
            if (SDL_GetTicks() > t_down_time)
            {
                t_down_time += fall_speed;
                tetrimino->moveAction(MOVE_DOWN);
            }
        }
        else
        {
            phase = LOCK_PHASE;
            lockDownTimer = SDL_GetTicks();
        }
    }
    if (phase == LOCK_PHASE)
    {
        if (canFall())
        {
            phase = FALLING_PHASE;
            t_down_time = SDL_GetTicks();
        }
        else
        {
            if (SDL_GetTicks() - lockDownTimer >= LOCK_DOWN_TIME)
            {
                lockTetrimino();
                phase = PATTERN_PHASE;
            }
        }
    }
    if (phase == PATTERN_PHASE)
    {
        rowsDestroyed = {};

        for (int row = 0; row < MATRIX_HEIGHT; row++)
        {
            bool wasRowDestroyed = true;

            for (int col = 0; col < MATRIX_WIDTH; col++)
            {
                if (!at(row, col).value())
                    wasRowDestroyed = false;
            }

            if (wasRowDestroyed)
                rowsDestroyed.push_back(row);
        }

        if (rowsDestroyed.empty())
            phase = GENERATION_PHASE;
        else
            phase = ANIMATION_PHASE;
    }

    if (phase == ANIMATION_PHASE)
    {
        for (int i = 0; i < rowsDestroyed.size(); i++)
        {
            int row = rowsDestroyed[i];
            for (int col = 0; col < MATRIX_WIDTH; col++)
            {
                Mino m = at(row, col);

                m.incrementAnimationProgress();
                
                set(row, col, m);

                if (m.animationEnded())
                {
                    phase = ELIMINATE_PHASE;
                    Mino empty;
                    set(row, col, empty);
                }	
            }
        }
    }
    if (phase == ELIMINATE_PHASE){
        for (int i = 0; i < rowsDestroyed.size(); i++){
            for (int j = rowsDestroyed[i]; j >= 0; j--){
                copyRow(j, j-1);
            }
        }

        phase = GENERATION_PHASE;
    }
}

void TetrisMap::copyRow(int row_dest, int row_source)
{
    for (int i = 0; i < MATRIX_WIDTH; i++){
        Mino source = at(row_source,i);
        set(row_dest,i,source);
    }
}

bool TetrisMap::canFall()
{
    array<Pos, 4> minos = tetrimino->getMinos();

    for (int i = 0; i < 4; i++) // Verifies for each mino if there's a collision
    {
        Pos pos(1, 0);
        pos += tetrimino->pos();
        pos += minos[i];
        if (at(pos).value() != 0)
            return false;
    }

    return true;
}

bool TetrisMap::validateTetriminoAction()
{
    array<Pos, 4> minos = tetrimino->getMinos();
    bool collides = false;

    for (int i = 0; i < 4; i++) // Verifies for each mino if there's a collision
    {
        if (at(tetrimino->pos() + minos[i]).value() != 0)
        {                                    // A mino is colliding with something
            tetrimino->undoPreviousAction(); // Undo rotation
            collides = true;
            break;
        }
    }
    if (!collides)
        lockDownTimer = SDL_GetTicks();
    // If there was no collision, the movement was validated.
    return !collides;
}

void TetrisMap::initTetriminoQueue()
{
    while (!virtualBag.empty())
    {
        int ti = rand() % virtualBag.size();
        char name = virtualBag.at(ti);
        virtualBag.erase(virtualBag.begin() + ti);

        tetriminoQueue.push_back(Tetrimino::generateTetriminoFrom(name));
    }

    virtualBag = VIRTUAL_BAG;
}
