#ifndef TETRIS_TETRIMINO_S_HPP
#define TETRIS_TETRIMINO_S_HPP

#include <tetrimino.hpp>

class TetriminoS : public Tetrimino
{
public:
    TetriminoS(Matrix &matrix);
    virtual bool tryMovementCollides();
    virtual map<Orientation, map<int, Pos>> getRotationMap();
};

#endif