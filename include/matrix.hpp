#ifndef TETRIS_MATRIX_HPP
#define TETRIS_MATRIX_HPP

#define MATRIX_WIDTH 10
#define MATRIX_HEIGHT 20

#include <pos.hpp>

class Matrix
{
public:
    Matrix();
    char at(Pos pos);
    char at(int row, int col);

private:
    // Matrix Height + 2 (Skyline) + 2 (Top wall) + 2 (Bottom wall)
    // Matrix Width + 2 (Left wall) + 2 (Right wall)
    // char can be: O, I, T, L, J, S, Z, W (for wall)
    char m_matrix[MATRIX_HEIGHT + 2 + 2 + 2][MATRIX_WIDTH + 2 + 2];
};

#endif