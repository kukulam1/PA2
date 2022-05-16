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

/**
 * @brief Apply random effects to bomb
 * @return CBomb& 
 */
CBomb & CBomb::AddEffect ( void )
{
    int rnd = rand() % 5;
    switch (rnd)
    {
    case (0):
        m_Time--;
        break;
    case (1):
        m_Time++;
        break;
    case (2):
    {
        CDoubleEffect double_eff;
        m_Effects.push_back( double_eff.Clone() );
        break;
    }
    case (3):
    {
        CFlameEffect flame_eff;
        m_Effects.push_back( flame_eff.Clone() );
        break;
    }
    case (4):
    {   
        CRoundEffect round_eff;
        m_Effects.push_back( round_eff.Clone() );
        break;
    }
    }
    return *this;
}

CBomb & CBomb::Boom ( CMap & map )
{
    for ( auto effect: m_Effects )
        effect->Effect( m_Coord, map );
    return *this;
}