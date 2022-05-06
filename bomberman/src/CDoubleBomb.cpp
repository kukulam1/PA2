/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#include "CDoubleBomb.h"

using namespace std;

CDoubleBomb::CDoubleBomb ()
: CBomb()
{}

CDoubleBomb::CDoubleBomb ( const CCoord & c )
: CBomb( c )
{}

unique_ptr<CBomb> CDoubleBomb::Clone () const
{
    return make_unique<CDoubleBomb>( *this );
}

//TODO
CDoubleBomb & CDoubleBomb::Effect ( CMap & map )
{
    //up
    for ( int i = m_Coord.m_X - 1; i >= (int)m_Coord.m_X - 2 && i >= 0; --i )
        map.Explode( CCoord( i, m_Coord.m_Y ));
    //down
    for ( int i = m_Coord.m_X + 1; i <= (int)m_Coord.m_X + 2 && i < (int)map.GetHeight(); ++i )
        map.Explode( CCoord( i, m_Coord.m_Y ));
    //left
    for ( int i = m_Coord.m_Y - 1; i >= (int)m_Coord.m_Y - 2 && i >= 0; --i )
        map.Explode( CCoord( m_Coord.m_X, i ));
    //right
    for ( int i = m_Coord.m_Y + 1; i <= (int)m_Coord.m_Y + 2 && i < (int)map.GetWidth(); ++i )
        map.Explode( CCoord( m_Coord.m_X, i ));
            
    map.m_Map[m_Coord.m_X][m_Coord.m_Y] = ' ';
    return *this;
}
