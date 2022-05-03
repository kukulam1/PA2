/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#pragma once
#include "CCoord.h"

struct CBomb 
{
    public:
        size_t m_Time;
        CCoord m_Coord;
        CBomb ( const CCoord & c )
        : m_Time ( 3 ), m_Coord ( c )
        {}
};