#include <tetrimino.hpp>
#include <tetriminoO.hpp>

TetriminoO::TetriminoO(Matrix &matrix) : Tetrimino(matrix){};

bool TetriminoO::tryMovementCollides()
{
    bool collides = false;
    Pos minos[4];
    switch (m_try_orientation)
    {
    case N:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(0, 2);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(1, 2);
    }
    break;
    case E:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(0, 2);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(1, 2);
    }
    break;
    case S:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(0, 2);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(1, 2);
    }
    break;
    case W:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(0, 2);
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

map<Orientation, map<int, Pos>> TetriminoO::getRotationMap()
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
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}},
        {E,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}},
        {S,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}}};
}
