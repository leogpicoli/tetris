#ifndef TETRIS_TETRIMINO_I_HPP
#define TETRIS_TETRIMINO_I_HPP

#include <tetrimino.hpp>

class TetriminoI : public Tetrimino
{
public:
    TetriminoI(Matrix &matrix);
    virtual bool tryMovementCollides();
    virtual map<Orientation, map<int, Pos>> getRotationMap();
};

#endif
