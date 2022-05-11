/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#include "CPlayer.h"

using namespace std;

/**
 * @brief Construct a new CPlayer::CPlayer object
 * place first player top left, second top right
 * @param num 
 * @param map_size 
 */
CPlayer::CPlayer ( size_t num, size_t map_size )
: m_Direction ( EMove::down ), m_Score ( 0 ), m_Num ( num )
{
    if ( num == 1 )
        m_Coord = CCoord( 0, 0 );
    else
        m_Coord = CCoord( 0, map_size - 1 );
    m_Bomb = make_shared<CBomb>();
}

CPlayer::CPlayer ( const CPlayer & c )
:   m_Coord     ( c.m_Coord ),
    m_Direction ( c.m_Direction ),
    m_Score     ( c.m_Score ),
    m_Bomb      ( std::move( c.m_Bomb )),
    m_Num       ( c.m_Num )             
{}

CCoord CPlayer::GetCoord () const
{
    return m_Coord;
}

char CPlayer::GetNum() const
{
    return m_Num + '0';
}

/**
 * @brief Get player a random power up.
 * @return CPlayer& 
 */
CPlayer & CPlayer::GetPowerUp ()
{
    int rnd = rand() % 3;
    if ( rnd == 0 )
    {
        CFlameBomb db;
        m_Bomb =  move( db.Clone() );
    }
    else if ( rnd == 1 )
    {
        CDoubleBomb db;
        m_Bomb =  move( db.Clone() );
    }
    else if ( rnd == 2 )
    {
        CAllDirectionBomb db;
        m_Bomb =  move( db.Clone() );        
    }
    return *this;
}