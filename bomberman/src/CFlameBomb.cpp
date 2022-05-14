/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#include "CFlameBomb.h"

using namespace std;

CFlameBomb::CFlameBomb ()
: CBomb()
{}

CFlameBomb::CFlameBomb ( const CCoord & c )
: CBomb( c )
{}

unique_ptr<CBomb> CFlameBomb::Clone () const
{
    return make_unique<CFlameBomb>( *this );
}

CFlameBomb & CFlameBomb::Effect ( CMap & map )
{
    //up
    for ( int i = m_Coord.m_X - 1; i >= 0; --i )
    {
        if ( 'A' <= map.m_Map[i][m_Coord.m_Y] && map.m_Map[i][m_Coord.m_Y] <= 'Z' )
            break;
        map.Explode( CCoord( i, m_Coord.m_Y ));
    }   
    //down
    for ( int i = m_Coord.m_X + 1; i < (int)map.GetHeight(); ++i )
    {
        if ( 'A' <= map.m_Map[i][m_Coord.m_Y] && map.m_Map[i][m_Coord.m_Y] <= 'Z' )
            break;
        map.Explode( CCoord( i, m_Coord.m_Y ));
    }  
    //left
    for ( int i = m_Coord.m_Y - 1; i >= 0; --i )
    {
        if ( 'A' <= map.m_Map[m_Coord.m_X][i] && map.m_Map[m_Coord.m_X][i] <= 'Z' )
            break;
        map.Explode( CCoord( m_Coord.m_X, i ));
    }
    //right
    for ( int i = m_Coord.m_Y + 1; i < (int)map.GetWidth(); ++i )
    {
        if ( 'A' <= map.m_Map[m_Coord.m_X][i] && map.m_Map[m_Coord.m_X][i] <= 'Z' )
            break;
        map.Explode( CCoord( m_Coord.m_X, i ));
    }
            
    map.m_Map[m_Coord.m_X][m_Coord.m_Y] = ' ';
    return *this;
}
