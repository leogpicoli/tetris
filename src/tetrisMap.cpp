#include <tetrisMap.hpp>
#include <pos.hpp>
#include <color.hpp>
#include <SDL2/SDL.h>
#include <iostream>

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
            if (col < 2 || col >= MATRIX_WIDTH +2 || row < 2 || row >= MATRIX_HEIGHT + 4)
            {
                m_matrix[row][col] = 'W';
            }
            else
            {
                m_matrix[row][col] = 0;
            }
        }
    }

    tetrimino = new TetriminoI();
    tetriminoAction(MOVE_LEFT);
}

char TetrisMap::at(int row, int col)
{
    // Since tetrominoes start at position -2 and -1 (positions representing the skyline)
    // And since we have 2 layers of walls above the skyline, we need to add 4 positions to the row
    // So, row -2 is actually represented as row 2 in the matrix, row 0 is actually row 4 in the matrix, etc...
    // Since we have 2 layers of wall in the left side we have to add 2 lines to the column;
    return m_matrix[row + 4][col + 2];
}

char TetrisMap::at(Pos p)
{
    return at(p.row(), p.col());
}

void TetrisMap::draw(SDL_Renderer *renderer) {
    for (auto row = 0; row < MATRIX_HEIGHT; row++) {
        for (auto col = 0; col < MATRIX_WIDTH; col++) {
            SDL_Rect rect;
            rect.x = col*TILE_SIZE;
            rect.y = row*TILE_SIZE;
            rect.w = TILE_SIZE;
            rect.h = TILE_SIZE;

            Color color = Color::fromTetrimino(at(row, col));

            SDL_SetRenderDrawColor(renderer, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
            SDL_RenderFillRect(renderer, &rect);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 150);
            SDL_RenderDrawRect(renderer, &rect);
        }
    }

    tetrimino->draw(renderer);
}

void TetrisMap::tetriminoAction(TetriminoAction action) {
    // Move action
    if (action == MOVE_DOWN || action == MOVE_RIGHT || action == MOVE_LEFT || action == MOVE_UP) {
        tetrimino->moveAction(action);
        validateTetriminoAction();
    } else { // Rotate action
        for (int i = 1;i <= 5;i++) { // Test each rotation point
            tetrimino->rotateAction(action, i);

            // If it is a valid rotation, stop the for that tests each rotation point.
            if (validateTetriminoAction()) break;
        }
    }
}

bool TetrisMap::validateTetriminoAction() {
    array<Pos, 4> minos = tetrimino->getMinos();
    bool collides = false;

    for (int i = 0; i < 4; i++) // Verifies for each mino if there's a collision
    {   
        if (at(tetrimino->pos() + minos[i]) != 0) { // A mino is colliding with something
            tetrimino->undoPreviousAction(); // Undo rotation
            collides = true;
            break;
        }
    }

    // If there was no collision, the movement was validated.
    return !collides;
}
