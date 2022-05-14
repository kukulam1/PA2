/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#include "CBomb.h"

using namespace std;

CBomb::CBomb ()
: m_Time ( 4 )
{
    CBasicEffect basic_eff;
    m_Effects.push_back( basic_eff.Clone() );
}


CBomb & CBomb::AddEffect ( void )
{
    CDoubleEffect cde;
    m_Effects.push_back( cde.Clone() );
    return *this;
}

CBomb & CBomb::Boom ( CMap & map )
{
    for ( auto effect: m_Effects )
        effect->Effect(  m_Coord, map, m_Time );
    return *this;
}