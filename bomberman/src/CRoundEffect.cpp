/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#include "CEffect.h"

using namespace std;

std::unique_ptr<CEffect> CRoundEffect::Clone ( void ) const
{
    return make_unique<CRoundEffect>( *this );
}

void CRoundEffect::Effect ( const CCoord & coord, CMap & map, size_t & bomb_time  ) const
{
    //up
    if ( coord.m_X != 0 )
    {
        map.Explode( CCoord( coord.m_X - 1, coord.m_Y ));
        //left
        if ( coord.m_Y != 0 )
        {
            map.Explode( CCoord( coord.m_X - 1, coord.m_Y - 1 ));
            map.Explode( CCoord( coord.m_X, coord.m_Y - 1 ));
        }
        //right
        if ( coord.m_Y != map.GetWidth() - 1 )
        {
            map.Explode( CCoord( coord.m_X - 1, coord.m_Y + 1 ));
            map.Explode( CCoord( coord.m_X, coord.m_Y + 1 ));
        }

    }
    //down
    if ( coord.m_X != map.GetHeight() - 1 )
    {
        map.Explode( CCoord( coord.m_X + 1, coord.m_Y ));
        //left
        if ( coord.m_Y != 0 )
        {
            map.Explode( CCoord( coord.m_X + 1, coord.m_Y - 1 ));
        }
        //right
        if ( coord.m_Y != map.GetWidth() - 1 )
        {
            map.Explode( CCoord( coord.m_X + 1, coord.m_Y + 1 ));
        }
    }
    map.m_Map[coord.m_X][coord.m_Y] = ' ';
}