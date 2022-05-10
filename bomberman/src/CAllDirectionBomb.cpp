/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#include "CAllDirectionBomb.h"

using namespace std;

CAllDirectionBomb::CAllDirectionBomb ()
: CBomb()
{}

CAllDirectionBomb::CAllDirectionBomb ( const CCoord & c )
: CBomb( c )
{}

unique_ptr<CBomb> CAllDirectionBomb::Clone () const
{
    return make_unique<CAllDirectionBomb>( *this );
}

//TODO
CAllDirectionBomb & CAllDirectionBomb::Effect ( CMap & map )
{
    //up
    if ( m_Coord.m_X != 0 )
    {
        map.Explode( CCoord( m_Coord.m_X - 1, m_Coord.m_Y ));
        //left
        if ( m_Coord.m_Y != 0 )
        {
            map.Explode( CCoord( m_Coord.m_X - 1, m_Coord.m_Y - 1 ));
            map.Explode( CCoord( m_Coord.m_X, m_Coord.m_Y - 1 ));
        }
        //right
        if ( m_Coord.m_Y != map.GetWidth() - 1 )
        {
            map.Explode( CCoord( m_Coord.m_X - 1, m_Coord.m_Y + 1 ));
            map.Explode( CCoord( m_Coord.m_X, m_Coord.m_Y + 1 ));
        }

    }
    //down
    if ( m_Coord.m_X != map.GetHeight() - 1 )
    {
        map.Explode( CCoord( m_Coord.m_X + 1, m_Coord.m_Y ));
        //left
        if ( m_Coord.m_Y != 0 )
        {
            map.Explode( CCoord( m_Coord.m_X + 1, m_Coord.m_Y - 1 ));
        }
        //right
        if ( m_Coord.m_Y != map.GetWidth() - 1 )
        {
            map.Explode( CCoord( m_Coord.m_X + 1, m_Coord.m_Y + 1 ));
        }
    }
    map.m_Map[m_Coord.m_X][m_Coord.m_Y] = ' ';

    return *this;
}
