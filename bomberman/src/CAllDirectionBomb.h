/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#pragma once

#include <memory>
#include "CBomb.h"

/**
 * @brief Bomb that burn all spots near 
 */
class CAllDirectionBomb : public CBomb
{
    public:
        CAllDirectionBomb ();

        CAllDirectionBomb ( const CCoord & c );

        std::unique_ptr<CBomb> Clone () const override;

        CAllDirectionBomb & Effect ( CMap & map ) override;

        const char GetVisual () const override { return 'A'; }       
};