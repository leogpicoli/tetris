#include <tetrimino.hpp>
#include <tetriminoI.hpp>

TetriminoI::TetriminoI(Matrix &matrix) : Tetrimino(matrix){};

bool TetriminoI::tryMovementCollides()
{
    bool collides = false;
    Pos minos[4];
    switch (m_try_orientation)
    {
    case N:
    {
        minos[0] = Pos(1, 0);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(1, 2);
        minos[3] = Pos(1, 3);
    }
    break;
    case E:
    {
        minos[0] = Pos(0, 2);
        minos[1] = Pos(1, 2);
        minos[2] = Pos(2, 2);
        minos[3] = Pos(3, 2);
    }
    break;
    case S:
    {
        minos[0] = Pos(2, 0);
        minos[1] = Pos(2, 1);
        minos[2] = Pos(2, 2);
        minos[3] = Pos(2, 3);
    }
    break;
    case W:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(2, 1);
        minos[3] = Pos(3, 1);
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

map<Orientation, map<int, Pos>> TetriminoI::getRotationMap()
{
    return {
        {N,
         {{1, Pos(1, 1)},
          {2, Pos(1, 0)},
          {3, Pos(1, 3)},
          {4, Pos(1, 0)},
          {5, Pos(1, 3)}}},
        {W,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(3, 1)},
          {5, Pos(0, 1)}}},
        {E,
         {{1, Pos(1, 1)},
          {2, Pos(1, 2)},
          {3, Pos(1, 2)},
          {4, Pos(0, 2)},
          {5, Pos(3, 2)}}},
        {S,
         {{1, Pos(1, 1)},
          {2, Pos(1, 3)},
          {3, Pos(1, 0)},
          {4, Pos(2, 3)},
          {5, Pos(2, 0)}}}};
}
