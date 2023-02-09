#ifndef TETRIS_TETRIMINO_L_HPP
#define TETRIS_TETRIMINO_L_HPP

#include <tetrimino.hpp>

class TetriminoL : public Tetrimino
{
public:
    TetriminoL(Matrix &matrix);
    virtual bool tryMovementCollides();
    virtual map<Orientation, map<int, Pos>> getRotationMap();
};

#endif