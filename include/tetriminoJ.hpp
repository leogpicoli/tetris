#ifndef TETRIS_TETRIMINO_J_HPP
#define TETRIS_TETRIMINO_J_HPP

#include <tetrimino.hpp>

class TetriminoJ : public Tetrimino
{
public:
    TetriminoJ(Matrix &matrix);
    virtual bool tryMovementCollides();
    virtual map<Orientation, map<int, Pos>> getRotationMap();
};

#endif