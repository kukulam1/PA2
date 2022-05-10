/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#pragma once
#include <memory>
#include "CCoord.h"
#include "CMap.h"

/**
 * @brief Class CBomb
 * virtual method Effect -> differenct effects
 */
class CBomb 
{
    public:
        size_t m_Time;
        CCoord m_Coord;

        CBomb ();

        CBomb ( const CCoord & c );

        virtual std::unique_ptr<CBomb> Clone () const;

        virtual CBomb & Effect ( CMap & map );

        virtual const char GetVisual () const { return 'O'; }
    private:        
};