#ifndef TETRIS_TETRIMINO_HPP
#define TETRIS_TETRIMINO_HPP

#include <color.hpp>
#include <pos.hpp>
#include <matrix.hpp>
#include <map>

using namespace std;

enum Orientation
{
    N, // 0
    W, // 1
    S, // 2
    E // 3
};

class Tetrimino
{
public:
    Tetrimino(Matrix &matrix);
    // Direction of rotation: -1 counter clockwise and 1 for clockwise
    void rotate(int direction);

    void moveLeft();
    void moveRight();
    void moveDown();
    void moveUp();

    virtual bool tryMovementCollides() = 0;
    virtual map<Orientation, map<int, Pos>> getRotationMap() = 0;

    void applyTryMovement();
    void beginTryMovement();

protected:
    Pos m_pos;
    Pos m_try_pos;

    Orientation m_orientation; // 0 for East, 1 for North, 2 for West, 3 for South
    Orientation m_try_orientation;

    Color m_color;

    Matrix &m_matrix;
};

#endif