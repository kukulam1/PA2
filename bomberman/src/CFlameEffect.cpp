/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#include "CEffect.h"

using namespace std;

std::unique_ptr<CEffect> CFlameEffect::Clone ( void ) const
{
    return make_unique<CFlameEffect>( *this );
}

void CFlameEffect::Effect ( const CCoord & coord, CMap & map, size_t & bomb_time  ) const
{
    //up
    for ( int i = coord.m_X - 1; i >= 0; --i )
    {
        if ( 'A' <= map.m_Map[i][coord.m_Y] && map.m_Map[i][coord.m_Y] <= 'Z' )
            break;
        map.Explode( CCoord( i, coord.m_Y ));
    }   
    //down
    for ( int i = coord.m_X + 1; i < (int)map.GetHeight(); ++i )
    {
        if ( 'A' <= map.m_Map[i][coord.m_Y] && map.m_Map[i][coord.m_Y] <= 'Z' )
            break;
        map.Explode( CCoord( i, coord.m_Y ));
    }  
    //left
    for ( int i = coord.m_Y - 1; i >= 0; --i )
    {
        if ( 'A' <= map.m_Map[coord.m_X][i] && map.m_Map[coord.m_X][i] <= 'Z' )
            break;
        map.Explode( CCoord( coord.m_X, i ));
    }
    //right
    for ( int i = coord.m_Y + 1; i < (int)map.GetWidth(); ++i )
    {
        if ( 'A' <= map.m_Map[coord.m_X][i] && map.m_Map[coord.m_X][i] <= 'Z' )
            break;
        map.Explode( CCoord( coord.m_X, i ));
    }
    map.m_Map[coord.m_X][coord.m_Y] = ' ';
}