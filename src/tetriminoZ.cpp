#include <tetrimino.hpp>
#include <tetriminoZ.hpp>

TetriminoT::TetriminoZ(Matrix &matrix) : Tetrimino(matrix){};

bool TetriminoT::tryMovementCollides()
{
    bool collides = false;
    Pos minos[4];
    switch (m_try_orientation)
    {
    case N:
    {
        minos[0] = Pos(0, 0);
        minos[1] = Pos(0, 1);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(1, 2);
    }
    break;
    case E:
    {
        minos[0] = Pos(0, 2);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(1, 2);
        minos[3] = Pos(2, 1);
    }
    break;
    case S:
    {
        minos[0] = Pos(1, 0);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(2, 1);
        minos[3] = Pos(2, 2);
    }
    break;
    case W:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(1, 0);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(1, 2);
    }
    break;
    default:
        break;
    }

    for (int i = 0; i < 4; i++)
    {
        collides |= m_matrix.at(m_try_pos + minos[i]);
    }

    return collides;
}

map<Orientation, map<int, Pos>> TetriminoZ::getRotationMap()
{
    return {
        {N,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}},
        {W,
         {{1, Pos(1, 1)},
          {2, Pos(1, 0)},
          {3, Pos(2, 0)},
          {4, Pos(-1, 1)},
          {5, Pos(-1, 0)}}},
        {E,
         {{1, Pos(1, 1)},
          {2, Pos(1, 2)},
          {3, Pos(2, 2)},
          {4, Pos(-1, 1)},
          {5, Pos(-1, 2)}}},
        {S,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}}};
}
