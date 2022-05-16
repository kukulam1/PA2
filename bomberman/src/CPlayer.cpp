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
: m_Direction ( EMove::left ), m_Score ( 0 ), m_Bomb (), m_Num ( num )
{
    if ( num == 1 )
        m_Coord = CCoord( 0, 0 );
    else
        m_Coord = CCoord( 0, map_size - 1 );
}

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
    m_Bomb.AddEffect();
    return *this;
}