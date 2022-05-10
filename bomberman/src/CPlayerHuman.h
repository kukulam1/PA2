/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#pragma once

#include <memory>
#include <iostream>
#include "CPlayer.h"
#include "EMove.h"
#include "CMap.h"

class CPlayerHuman : public CPlayer
{
    public:
        CPlayerHuman ( size_t num, size_t map_size );

        std::unique_ptr<CPlayer> Clone () const override;

        EMove GetMove ( const CMap & map ) override;
    private:
};