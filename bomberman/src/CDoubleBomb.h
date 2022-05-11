/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#pragma once

#include <memory>
#include "CBomb.h"

/**
 * @brief Bomb that burn all spots with distance 2
 */
class CDoubleBomb : public CBomb
{
    public:
        CDoubleBomb ();

        CDoubleBomb ( const CCoord & c );

        std::unique_ptr<CBomb> Clone () const override;

        CDoubleBomb & Effect ( CMap & map ) override;

        const char GetVisual () const override { return 'D'; }
        
};