/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#pragma once

#include <cstddef>

struct CCoord
{
    public:
        CCoord() = default;
        CCoord ( size_t x, size_t y )
        : m_X( x ), m_Y( y )
        {}
        size_t m_X;
        size_t m_Y;
    private:
};