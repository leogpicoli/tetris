#ifndef TETRIS_TETRIMINO_Z_HPP
#define TETRIS_TETRIMINO_Z_HPP

#include <tetrimino.hpp>

class TetriminoZ : public Tetrimino
{
public:
    TetriminoZ(Matrix &matrix);
    virtual bool tryMovementCollides();
    virtual map<Orientation, map<int, Pos>> getRotationMap();
};

#endif