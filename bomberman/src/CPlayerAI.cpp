/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#include "CPlayerAI.h"

using namespace std;

CPlayerAI::CPlayerAI ( size_t num, size_t map_size )
: CPlayer( num, map_size )
{
}

std::unique_ptr<CPlayer> CPlayerAI::Clone () const 
{
    return make_unique<CPlayerAI>( *this );
}

/**
 * @brief implements dummz AI move:
 * 1) if bomb is close -> run from it
 * 2) otherwise make random move 
 * @param map 
 * @return EMove 
 */
EMove CPlayerAI::GetMove ( const CMap & map ) 
{
    if ( map.WillExplodeClose( m_Coord ) )
    {
        return RunFromBomb( map );
    }
    return RandomMove();
}

EMove CPlayerAI::RunFromBomb ( const CMap & map ) 
{
    //up
    if ( (int)m_Coord.m_X - 1 >= 0 )
    {
        if ( map.m_Map[m_Coord.m_X - 1][m_Coord.m_Y] == 'O' || map.m_Map[m_Coord.m_X - 1][m_Coord.m_Y] == 'A' )
            return EMove::down;
    }
    //down
    if ( m_Coord.m_X + 1 < map.GetHeight() )
    {
        if ( map.m_Map[m_Coord.m_X + 1][m_Coord.m_Y] == 'O' || map.m_Map[m_Coord.m_X + 1][m_Coord.m_Y] == 'A' )
            return EMove::up;
    }
    //left
    if ( (int)m_Coord.m_Y - 1 >= 0 )
    {
        if ( map.m_Map[m_Coord.m_X][m_Coord.m_Y - 1] == 'O' || map.m_Map[m_Coord.m_X][m_Coord.m_Y - 1] == 'A' )
            return EMove::right;
    }
    //right
    if ( m_Coord.m_Y + 1 < map.GetWidth() )
    {
        if ( map.m_Map[m_Coord.m_X][m_Coord.m_Y + 1] == 'O' || map.m_Map[m_Coord.m_X][m_Coord.m_Y + 1] == 'A' )
            return EMove::left;
    }
    throw "Bomb Close foun in map not with player";
    return RandomMove();
}

EMove CPlayerAI::RandomMove() const
{
    int rnd = rand() % 5;
    if ( rnd == 0 )
        return EMove::left;
    if ( rnd == 1 )
        return EMove::right;
    if ( rnd == 2 )
        return EMove::up;
    if ( rnd == 3 )
        return EMove::down;
    return EMove::bomb;    
}
