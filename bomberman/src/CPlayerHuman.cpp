/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#include <cstddef>
#include <iostream>
#include <memory>
#include "CPlayerHuman.h"

using namespace std;

CPlayerHuman::CPlayerHuman ( size_t num, size_t map_size )
: CPlayer( num, map_size )
{
}

std::unique_ptr<CPlayer> CPlayerHuman::Clone () const 
{
    return make_unique<CPlayerHuman>( *this );
}

EMove CPlayerHuman::GetMove ( const CMap & map ) 
{
    cout << "Player " << m_Num << ": ";
    char c;
    cin >> c;
    return EMove(c);
}