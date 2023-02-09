#ifndef TETRIS_TETRIMINO_O_HPP
#define TETRIS_TETRIMINO_O_HPP

#include <tetrimino.hpp>

class TetriminoO : public Tetrimino
{
public:
    TetriminoO(Matrix &matrix);
    virtual bool tryMovementCollides();
    virtual map<Orientation, map<int, Pos>> getRotationMap();
};

#endif