/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#include "CBomb.h"

using namespace std;

CBomb::CBomb ()
: m_Time ( 4 )
{}

CBomb::CBomb ( const CCoord & c )
: m_Time ( 4 ), m_Coord ( c )
{}

unique_ptr<CBomb> CBomb::Clone () const
{
    return make_unique<CBomb>( *this );
}

CBomb & CBomb::Effect ( CMap & map )
{
    //up
    if ( m_Coord.m_X != 0 )
    {
        map.Explode( CCoord( m_Coord.m_X - 1, m_Coord.m_Y ));
    }
    //down
    if ( m_Coord.m_X != map.GetHeight() - 1 )
    {
        map.Explode( CCoord( m_Coord.m_X + 1, m_Coord.m_Y ));
    }
    //left
    if ( m_Coord.m_Y != 0 )
    {
        map.Explode( CCoord( m_Coord.m_X, m_Coord.m_Y - 1 ));
    }
    //right
    if ( m_Coord.m_Y != map.GetWidth() - 1 )
    {
        map.Explode( CCoord( m_Coord.m_X, m_Coord.m_Y + 1 ));
    }
    map.m_Map[m_Coord.m_X][m_Coord.m_Y] = ' ';

    return *this;
}