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

unique_ptr<CPlayer> CPlayerAI::Clone () const 
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
        return RunFromBomb( map );
    if ( CanPlaceClose( map ) )
        return EMove::bomb;
    return ChaseOpponent( map );
}

EMove CPlayerAI::RunFromBomb ( const CMap & map ) 
{
    //bomb up
    if ( (int)m_Coord.m_X - 1 >= 0 )
    {
        if ( map.m_Map[m_Coord.m_X - 1][m_Coord.m_Y] == 'O' )
        {
        //run down
            if ( m_Coord.m_X + 1 < map.GetHeight() && map.IsFree( CCoord( m_Coord.m_X + 1, m_Coord.m_Y)) )
                return EMove::down;
            else if ( m_Coord.m_Y + 1 < map.GetWidth() && map.IsFree( CCoord( m_Coord.m_X, m_Coord.m_Y + 1)) )
                return EMove::right;
            else
                return EMove::left;
        }
    }
    //bomb down
    if ( m_Coord.m_X + 1 < map.GetHeight() )
    {
        if ( map.m_Map[m_Coord.m_X + 1][m_Coord.m_Y] == 'O' )
        {
        //run up
            if ( (int)m_Coord.m_X - 1 > 0 && map.IsFree( CCoord( m_Coord.m_X - 1, m_Coord.m_Y)) )
                return EMove::up;
            else if ( m_Coord.m_Y + 1 < map.GetWidth() && map.IsFree( CCoord( m_Coord.m_X, m_Coord.m_Y + 1)) )
                return EMove::right;
            else
                return EMove::left;
        }
    }
    //bomb left
    if ( (int)m_Coord.m_Y - 1 >= 0 )
    {
        if ( map.m_Map[m_Coord.m_X][m_Coord.m_Y - 1] == 'O')
        {
        // run right
            if ( m_Coord.m_Y + 1 < map.GetWidth() && map.IsFree( CCoord( m_Coord.m_X, m_Coord.m_Y + 1)) )
                return EMove::right;
        //run down
            else if ( m_Coord.m_X + 1 < map.GetHeight() && map.IsFree( CCoord( m_Coord.m_X + 1, m_Coord.m_Y)) )
                return EMove::down;
            else
                return EMove::up;
        }
    }
    //bomb right
    if ( m_Coord.m_Y + 1 < map.GetWidth() )
    {
        if ( map.m_Map[m_Coord.m_X][m_Coord.m_Y + 1] == 'O' )
        {
        // run right
            if ( (int)m_Coord.m_Y - 1 > 0 && map.IsFree( CCoord( m_Coord.m_X, m_Coord.m_Y - 1)) )
                return EMove::left;
        //run down
            else if ( m_Coord.m_X + 1 < map.GetHeight() && map.IsFree( CCoord( m_Coord.m_X + 1, m_Coord.m_Y)) )
                return EMove::down;
            else
                return EMove::up;
        }
    }
    throw "Bomb Close foun in map not with player";
    return RandomMove();
}

bool CPlayerAI::CanPlaceClose( const CMap & map ) const
{
    if ( m_Direction == EMove::down )
    {
        if ( m_Coord.m_X + 2 < map.GetHeight() )
        // opponent under bomb after placement
            if ( map.IsPlayer( CCoord( m_Coord.m_X + 2, m_Coord.m_Y )) )
                return true;
        if ( m_Coord.m_X + 1 < map.GetHeight() )
        {
        // opponent left from bomb after placement
            if ( (int)m_Coord.m_Y - 1 >= 0 && map.IsPlayer( CCoord( m_Coord.m_X + 1, m_Coord.m_Y - 1 )) )
                return true;
        // opponent right from bomb after placement
            if ( m_Coord.m_Y + 1 < map.GetWidth() && map.IsPlayer( CCoord( m_Coord.m_X + 1, m_Coord.m_Y + 1 )) )
                return true;
        }
    }
    if ( m_Direction == EMove::up )
    {
        if ( (int)m_Coord.m_X - 2 >= 0 )
        // opponent up from bomb after placement
            if ( map.IsPlayer( CCoord( m_Coord.m_X - 2, m_Coord.m_Y )) )
                return true;
        if ( (int)m_Coord.m_X - 1 >= 0 )
        {
        // opponent left from bomb after placement
            if ( (int)m_Coord.m_Y - 1 >= 0 && map.IsPlayer( CCoord( m_Coord.m_X - 1, m_Coord.m_Y - 1 )) )
                return true;
        // opponent right from bomb after placement
            if ( m_Coord.m_Y + 1 < map.GetWidth() && map.IsPlayer( CCoord( m_Coord.m_X - 1, m_Coord.m_Y + 1 )) )
                return true;
        }
    }
    if ( m_Direction == EMove::right )
    {
        if ( m_Coord.m_Y + 2 < map.GetWidth() )
        // opponent right from bomb after placement
            if ( map.IsPlayer( CCoord( m_Coord.m_X, m_Coord.m_Y + 2 )) )
                return true;
        if ( m_Coord.m_Y + 1 < map.GetWidth())
        {
        // opponent up from bomb after placement
            if ( (int)m_Coord.m_X - 1 >= 0 && map.IsPlayer( CCoord( m_Coord.m_X - 1, m_Coord.m_Y + 1 )) )
                return true;
        // opponent dwon from bomb after placement
            if ( m_Coord.m_X + 1 < map.GetHeight() && map.IsPlayer( CCoord( m_Coord.m_X + 1, m_Coord.m_Y + 1 )) )
                return true;
        }
    }
    if ( m_Direction == EMove::left )
    {
        if ( m_Coord.m_Y - 2 >= 0 )
        // opponent right from bomb after placement
            if ( map.IsPlayer( CCoord( m_Coord.m_X, m_Coord.m_Y - 2 )) )
                return true;
        if ( m_Coord.m_Y - 1 >= 0 )
        {
        // opponent up from bomb after placement
            if ( (int)m_Coord.m_X - 1 >= 0 && map.IsPlayer( CCoord( m_Coord.m_X - 1, m_Coord.m_Y - 1 )) )
                return true;
        // opponent dwon from bomb after placement
            if ( m_Coord.m_X + 1 < map.GetHeight() && map.IsPlayer( CCoord( m_Coord.m_X + 1, m_Coord.m_Y - 1 )) )
                return true;
        }
    }
    return false;
}

/**
 * @brief Chase oppponent
 * @param map 
 * @return EMove in direction of opponent, if not possible return random move
 */
EMove CPlayerAI::ChaseOpponent ( const CMap & map )
{
    CCoord enemy_coord = map.FindPlayer( '1' );

    if ( enemy_coord.m_X < m_Coord.m_X 
        && map.IsFree( CCoord( m_Coord.m_X - 1, m_Coord.m_Y))
        && !map.WillExplodeClose( CCoord( m_Coord.m_X - 1, m_Coord.m_Y)) ) 
        return EMove::up;
    if ( enemy_coord.m_X > m_Coord.m_X 
        && map.IsFree( CCoord( m_Coord.m_X + 1, m_Coord.m_Y))
        && !map.WillExplodeClose( CCoord( m_Coord.m_X + 1, m_Coord.m_Y)) ) 
        return EMove::down;
    if ( enemy_coord.m_Y < m_Coord.m_Y 
        && map.IsFree( CCoord( m_Coord.m_X, m_Coord.m_Y - 1))
        && !map.WillExplodeClose( CCoord( m_Coord.m_X, m_Coord.m_Y - 1)) ) 
        return EMove::left;
    if ( enemy_coord.m_Y > m_Coord.m_Y 
        && map.IsFree( CCoord( m_Coord.m_X, m_Coord.m_Y + 1))
        && !map.WillExplodeClose( CCoord( m_Coord.m_X, m_Coord.m_Y + 1)) ) 
        return EMove::right;

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
