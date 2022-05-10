/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#pragma once

#include <memory>
#include "CBomb.h"

class CFlameBomb : public CBomb
{
    public:
        CFlameBomb ();

        CFlameBomb ( const CCoord & c );

        std::unique_ptr<CBomb> Clone () const override;

        CFlameBomb & Effect ( CMap & map ) override;

        const char GetVisual () const override { return 'F'; }
};