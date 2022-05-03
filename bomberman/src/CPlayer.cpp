/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#include <iostream>
#include "CPlayer.h"

using namespace std;

CPlayer::CPlayer ( size_t num, size_t map_size )
: m_Direction ( EMove::down ), m_Num ( num ), m_Alive ( true )
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