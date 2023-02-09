#include <tetrimino.hpp>
#include <color.hpp>
#include <matrix.hpp>

Tetrimino::Tetrimino(Matrix &matrix) : m_matrix(matrix)
{
    m_orientation = N;
}

// direction: -1 for counter clockwise, 1 for clockwise
void Tetrimino::rotate(int direction)
{
    beginTryMovement();

    m_try_orientation = Orientation((4 + m_try_orientation + direction) % 4);

    auto rotationMap = getRotationMap();

    for (int i = 1; i <= 5; i++)
    {
        Pos pos_initial = rotationMap[m_try_orientation][i]; // Pos(3,1)
        Pos pos_final = rotationMap[m_orientation][i]; // Pos(1,0)
        Pos shift = pos_final - pos_initial; // Pos(-2, -1)

        m_try_pos += shift;

        if (!tryMovementCollides()) break;
        
        // redo operation of shifting
        m_try_pos -= shift;
    }

    applyTryMovement();
}

void Tetrimino::moveLeft()
{
    beginTryMovement();
    m_try_pos.col() -= 1;
    applyTryMovement();
}

void Tetrimino::moveRight()
{
    beginTryMovement();
    m_try_pos.col() += 1;
    applyTryMovement();
}

void Tetrimino::moveDown()
{
    beginTryMovement();
    m_try_pos.row() += 1;
    applyTryMovement();
}

void Tetrimino::moveUp()
{
    beginTryMovement();
    m_try_pos.row() -= 1;
    applyTryMovement();
}

void Tetrimino::beginTryMovement()
{
    m_try_orientation = m_orientation;
    m_try_pos = m_pos;
}

void Tetrimino::applyTryMovement()
{
    if (!tryMovementCollides())
    {
        m_orientation = m_try_orientation;
        m_pos = m_try_pos;
    }
}