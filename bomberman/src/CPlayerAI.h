/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#pragma once

#include <memory>
#include "CPlayer.h"
#include "EMove.h"
#include "CMap.h"

class CPlayerAI : public CPlayer
{
    public:
        CPlayerAI ( size_t num, size_t map_size );

        std::unique_ptr<CPlayer> Clone () const override;

        EMove GetMove ( const CMap & map ) override;

    private:
        EMove RunFromBomb ( const CMap & map );

        EMove RandomMove () const;

        EMove ChaseOpponent ( const CMap & map );
        
        bool BombClose ( const CMap & map ) const;
        
        bool OppClose ( const CMap & map ) const;
        
        bool BoxClose ( const CMap & map ) const;

        bool CanPlaceClose ( const CMap & map ) const;
};