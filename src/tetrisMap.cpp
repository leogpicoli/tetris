#include <tetrisMap.hpp>
#include <pos.hpp>
#include <color.hpp>
#include <SDL2/SDL.h>
#include <iostream>
#include <tetrimino.hpp>
#include <globals.hpp>
#include <cmath>

TetrisMap::TetrisMap()
{
    text = new Text();

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
        SDL_RenderDrawLine(
            renderer,
            TETRIS_MAP_INIT_X,
            row * TILE_SIZE + TETRIS_MAP_INIT_Y,
            TETRIS_MAP_WIDTH + TETRIS_MAP_INIT_X,
            row * TILE_SIZE + TETRIS_MAP_INIT_Y);
    }

    for (auto col = 0; col <= MATRIX_WIDTH; col++)
    {
        SDL_RenderDrawLine(
            renderer,
            TETRIS_MAP_INIT_X + col * TILE_SIZE,
            TETRIS_MAP_INIT_Y,
            col * TILE_SIZE + TETRIS_MAP_INIT_X,
            TETRIS_MAP_HEIGHT + TETRIS_MAP_INIT_Y);
    }

    tetrimino->draw(renderer);

    for (auto row = 0; row < MATRIX_HEIGHT; row++)
    {
        for (auto col = 0; col < MATRIX_WIDTH; col++)
        {
            Mino m = at(row, col);
            int x = col * TILE_SIZE + TETRIS_MAP_INIT_X;
            int y = row * TILE_SIZE + TETRIS_MAP_INIT_Y;
            m.draw(renderer, x, y);
        }
    }

    if (phase == FALLING_PHASE)
        drawGhostMinos(renderer);

    drawQueueTetriminos(renderer);

    drawTetriminoHold(renderer);

    drawGameStatus(renderer);
}

void TetrisMap::tetriminoAction(TetriminoAction action)
{
    // Move action
    if (action == DROP)
    {
        while (validateTetriminoAction())
        {
            tetrimino->moveAction(MOVE_DOWN);
            score += 2;
        }
        lockTetrimino();
        phase = PATTERN_PHASE;
    }
    else if (action == MOVE_DOWN || action == MOVE_RIGHT || action == MOVE_LEFT || action == MOVE_UP)
    {
        tetrimino->moveAction(action);
        bool val = validateTetriminoAction();
        if (val && action == MOVE_DOWN)
            score += 1;
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

        canChangeHold = true;
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
        {
            phase = GENERATION_PHASE;
        }
        else
        {
            updateGameStatus(rowsDestroyed.size());
            phase = ANIMATION_PHASE;
        }
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
    if (phase == ELIMINATE_PHASE)
    {
        for (int i = 0; i < rowsDestroyed.size(); i++)
        {
            for (int j = rowsDestroyed[i]; j >= 0; j--)
            {
                copyRow(j, j - 1);
            }
        }

        phase = GENERATION_PHASE;
    }
}
void TetrisMap::drawGameStatus(SDL_Renderer *renderer)
{
    int y = WINDOW_HEIGHT / 2;
    text->drawStart(10, y, "SCORE", renderer);
    text->drawEnd(TETRIS_MAP_INIT_X - 10, y, to_string(score), renderer);

    y += 2 * TETRIS_MAP_MARGIN;
    text->drawStart(10, y, "LEVEL", renderer);
    text->drawEnd(TETRIS_MAP_INIT_X - 10, y, to_string(level), renderer);

    y += 2 * TETRIS_MAP_MARGIN;
    text->drawStart(10, y, "LINES", renderer);
    text->drawEnd(TETRIS_MAP_INIT_X - 10, y, to_string(lines_cleared), renderer);
}

void TetrisMap::updateGameStatus(int nbLinesCleared)
{
    // Update score
    switch (nbLinesCleared)
    {
    case 1:
        score += 100 * level;
        break;
    case 2:
        score += 300 * level;
        break;
    case 3:
        score += 500 * level;
        break;
    case 4:
        score += 800 * level;
        break;
    default:
        break;
    }

    // Update number of lines cleared
    lines_cleared += nbLinesCleared;

    // Update level
    level = lines_cleared / 10 + 1;

    // Update fall speed
    fall_speed = 1000 * pow(0.8 - (level - 1) * 0.007, level - 1);

    cout << score << endl;
}

void TetrisMap::copyRow(int row_dest, int row_source)
{
    for (int i = 0; i < MATRIX_WIDTH; i++)
    {
        Mino source = at(row_source, i);
        set(row_dest, i, source);
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

void TetrisMap::drawGhostMinos(SDL_Renderer *renderer)
{
    array<Pos, 4> posMino = tetrimino->getMinos();
    Pos posGhostTetrimino = tetrimino->pos();

    bool collision = false;
    int count = 0;

    while (!collision)
    {
        posGhostTetrimino.row() += 1;

        for (int i = 0; i < 4; i++)
        {
            Pos pos = posMino[i] + posGhostTetrimino;

            if (at(pos).value() != 0)
            {
                collision = true;
                break;
            }
            count += 1;
        }
    }

    posGhostTetrimino.row() -= 1;

    for (int i = 0; i < 4; i++)
    {
        Pos posToPaint = posGhostTetrimino + posMino[i];
        Mino minoToPaint('G'); // G for ghost

        int x = posToPaint.col() * TILE_SIZE + TETRIS_MAP_INIT_X;
        int y = posToPaint.row() * TILE_SIZE + TETRIS_MAP_INIT_Y;

        minoToPaint.draw(renderer, x, y);
    }
}

void TetrisMap::drawTetriminoHold(SDL_Renderer *renderer)
{
    text->drawCenter(0, TETRIS_MAP_MARGIN, TETRIS_MAP_INIT_X, TETRIS_MAP_MARGIN, "HOLD", renderer);
    if (tetriminoHold)
    {
        int x = (TETRIS_MAP_INIT_X - (tetriminoHold->getSize() * TILE_SIZE)) / 2;

        // Adjust vertical if is Tetrimino I
        int adjust_vertical = tetriminoHold->name() == 'I' ? 3 : 2;
        int y = (3.5 * TETRIS_MAP_MARGIN - (adjust_vertical * TILE_SIZE) / 2);
        tetriminoHold->draw(renderer, x, y);
    }
}

void TetrisMap::changeHold()
{
    if (canChangeHold)
    {
        if (tetriminoHold)
        {
            Tetrimino *aux = tetrimino;
            tetrimino = tetriminoHold;
            tetriminoHold = aux;
            tetrimino->reset();
        }
        else
        {
            tetriminoHold = tetrimino;
            generateNextTetrimino();
        }

        tetriminoHold->reset();

        t_down_time = SDL_GetTicks();
        canChangeHold = false;
    }
}

void TetrisMap::drawQueueTetriminos(SDL_Renderer *renderer)
{
    text->drawCenter(
        TETRIS_MAP_INIT_X + TETRIS_MAP_WIDTH,
        TETRIS_MAP_MARGIN,
        WINDOW_WIDTH,
        TETRIS_MAP_MARGIN, "NEXT", renderer);
    for (int i = 0; i < 4; i++)
    {
        Tetrimino *t = tetriminoQueue[i];
        int x = TETRIS_MAP_INIT_X + TETRIS_MAP_WIDTH + (TETRIS_MAP_INIT_X - (t->getSize() * TILE_SIZE)) / 2;
        // Adjust vertical if is Tetrimino I
        int adjust_vertical = t->name() == 'I' ? 3 : 2;
        int y = 3.5 * TETRIS_MAP_MARGIN + TETRIMINO_QUEUE_MARGIN * i - (adjust_vertical * TILE_SIZE) / 2;
        t->draw(renderer, x, y);
    }
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
