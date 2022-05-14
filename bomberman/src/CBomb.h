/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#pragma once
#include <memory>
#include <vector>
#include "CCoord.h"
#include "CMap.h"
#include "CEffect.h"

/**
 * @brief Class CBomb
 * virtual method Effect -> differenct effects
 */
class CBomb 
{
    public:
        CCoord m_Coord;
        size_t m_Time;

        CBomb ();

        CBomb & AddEffect ();

        CBomb & Boom ( CMap & map );

    private:
        std::vector<std::shared_ptr<CEffect>> m_Effects;
};