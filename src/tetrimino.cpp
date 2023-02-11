#include <tetrimino.hpp>
#include <color.hpp>
#include <globals.hpp>

Tetrimino::Tetrimino()
{
    m_orientation = N;
}


TetriminoI::TetriminoI() : Tetrimino(){
    m_color = CYAN;
    t_name = 'I';
};

TetriminoJ::TetriminoJ() : Tetrimino(){
    m_color = BLUE;
    t_name = 'J';
};

TetriminoL::TetriminoL() : Tetrimino(){
    m_color = ORANGE;
    t_name = 'L';
};

TetriminoO::TetriminoO() : Tetrimino(){
    m_color = YELLOW;
    t_name = 'O';
};

TetriminoT::TetriminoT() : Tetrimino(){
    m_color = PURPLE;
    t_name = 'T';
};

TetriminoS::TetriminoS() : Tetrimino(){
    m_color = GREEN;
    t_name = 'S';
};

TetriminoZ::TetriminoZ() : Tetrimino(){
    m_color = RED;
    t_name = 'Z';
};


void Tetrimino::rotateAction(TetriminoAction rot, int point)
{
    prev_pos = m_pos;
    prev_orientation = m_orientation;

    int dir = (rot == ROTATE_LEFT ? -1 : 1);

    m_orientation = Orientation((4 + m_orientation + dir) % 4);

    auto rotationMap = getRotationMap();

    Pos pos_initial = rotationMap[m_orientation][point]; // Pos(3,1)
    Pos pos_final = rotationMap[prev_orientation][point]; // Pos(1,0)
    Pos shift = pos_final - pos_initial; // Pos(-2, -1)

    m_pos += shift;
}

void Tetrimino::moveAction(TetriminoAction mov)
{   
    prev_pos = m_pos;
    prev_orientation = m_orientation;

    switch (mov) {
        case MOVE_LEFT: m_pos.col() -= 1;
            break;
        case MOVE_RIGHT: m_pos.col() += 1;
            break;
        case MOVE_DOWN: m_pos.row() += 1;
            break;
        case MOVE_UP: m_pos.row() -= 1;
            break;
        default:
            break;
    }
}

void Tetrimino::undoPreviousAction()
{   
    m_pos = prev_pos;
    m_orientation = prev_orientation;
}

Pos &Tetrimino::pos()
{
    return m_pos;
}

const Pos &Tetrimino::pos() const
{
    return m_pos;
}

void Tetrimino::draw(SDL_Renderer *renderer) {
    array<Pos, 4> minos = getMinos();

    SDL_Rect rect;

    for (int i = 0;i < 4;i++) {
        Pos real_pos = minos[i] + m_pos;
        
        rect.x = real_pos.col()*TILE_SIZE;
        rect.y = real_pos.row()*TILE_SIZE;
        rect.w = TILE_SIZE;
        rect.h = TILE_SIZE;

        SDL_SetRenderDrawColor(renderer, m_color.getRed(), m_color.getGreen(), m_color.getBlue(), m_color.getAlpha());
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 150);
        SDL_RenderDrawRect(renderer, &rect);

    }
}

array<Pos, 4> TetriminoJ::getMinos()
{
    array<Pos,4> minos;

    switch (m_orientation)
    {
    case N:
    {
        minos[0] = Pos(0, 0);
        minos[1] = Pos(1, 0);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(1, 2);
    }
    break;
    case E:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(0, 2);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(2, 1);
    }
    break;
    case S:
    {
        minos[0] = Pos(1, 0);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(1, 2);
        minos[3] = Pos(2, 2);
    }
    break;
    case W:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(2, 0);
        minos[3] = Pos(2, 1);
    }
    break;
    default:
        break;
    }

    return minos;
}

array<Pos, 4> TetriminoI::getMinos() {
    array<Pos,4> minos;

    switch (m_orientation)
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

    return minos;
}

array<Pos, 4> TetriminoL::getMinos()
{
    array<Pos,4> minos;
    switch (m_orientation)
    {
    case N:
    {
        minos[0] = Pos(0, 2);
        minos[1] = Pos(1, 0);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(1, 2);
    }
    break;
    case E:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(2, 1);
        minos[3] = Pos(2, 2);
    }
    break;
    case S:
    {
        minos[0] = Pos(1, 0);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(1, 2);
        minos[3] = Pos(2, 0);
    }
    break;
    case W:
    {
        minos[0] = Pos(0, 0);
        minos[1] = Pos(0, 1);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(2, 1);
    }
    break;
    default:
        break;
    }

    return minos;
}

array<Pos, 4> TetriminoO::getMinos()
{
    array<Pos,4> minos;
    switch (m_orientation)
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

    return minos;
}

array<Pos, 4> TetriminoS::getMinos()
{
    array<Pos,4> minos;
    switch (m_orientation)
    {
    case N:
    {
        minos[0] = Pos(0, 0);
        minos[1] = Pos(1, 0);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(1, 2);
    }
    break;
    case E:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(0, 2);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(2, 1);
    }
    break;
    case S:
    {
        minos[0] = Pos(1, 0);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(1, 2);
        minos[3] = Pos(2, 2);
    }
    break;
    case W:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(2, 0);
        minos[3] = Pos(2, 1);
    }
    break;
    default:
        break;
    }

    return minos;
}

array<Pos, 4> TetriminoT::getMinos()
{
    array<Pos,4> minos;
    switch (m_orientation)
    {
    case N:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(1, 0);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(1, 2);
    }
    break;
    case E:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(1, 2);
        minos[3] = Pos(2, 1);
    }
    break;
    case S:
    {
        minos[0] = Pos(1, 0);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(1, 2);
        minos[3] = Pos(2, 1);
    }
    break;
    case W:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(1, 0);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(2, 1);
    }
    break;
    default:
        break;
    }

    return minos;
}

array<Pos, 4> TetriminoZ::getMinos()
{
    array<Pos,4> minos;
    switch (m_orientation)
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
        minos[3] = Pos(2, 0);
    }
    break;
    default:
        break;
    }

    return minos;
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

map<Orientation, map<int, Pos>> TetriminoJ::getRotationMap()
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

map<Orientation, map<int, Pos>> TetriminoL::getRotationMap()
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

map<Orientation, map<int, Pos>> TetriminoS::getRotationMap()
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

map<Orientation, map<int, Pos>> TetriminoT::getRotationMap()
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
