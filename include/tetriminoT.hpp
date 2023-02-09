#ifndef TETRIS_TETRIMINO_T_HPP
#define TETRIS_TETRIMINO_T_HPP

#include <tetrimino.hpp>

class TetriminoT : public Tetrimino
{
public:
    TetriminoT(Matrix &matrix);
    virtual bool tryMovementCollides();
    virtual map<Orientation, map<int, Pos>> getRotationMap();
};

#endif