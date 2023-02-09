#include <matrix.hpp>
#include <pos.hpp>

Matrix::Matrix()
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
                WW  tetris  WW
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
            if (col < 2 || col >= MATRIX_WIDTH || row < 2 || row >= MATRIX_HEIGHT + 4)
            {
                m_matrix[row][col] = 'W';
            }
            else
            {
                m_matrix[row][col] = 0;
            }
        }
    }
}

char Matrix::at(int row, int col)
{
    // Since tetrominoes start at position -2 and -1 (positions representing the skyline)
    // And since we have 2 layers of walls above the skyline, we need to add 4 positions to the row
    // So, row -2 is actually represented as row 2 in the matrix, row 0 is actually row 4 in the matrix, etc...
    // Since we have 2 layers of wall in the left side we have to add 2 lines to the column;
    return m_matrix[row + 4][col + 2];
}

char Matrix::at(Pos p)
{
    return at(p.row(), p.col());
}