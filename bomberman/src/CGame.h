/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#pragma once

#include <vector>
#include <memory>
#include "CMap.h"
#include "CPlayer.h"
#include "CPlayerHuman.h"
#include "CBomb.h"

class CGame
{
    public:
        explicit CGame ( CMap map );

        CGame & Run ();

    private:
        CMap m_Map;
        std::vector<std::shared_ptr<CPlayer>> m_Players;
        std::vector<std::shared_ptr<CBomb>> m_Bombs;
        size_t m_Alive = 0;

        CGame & Init ();
        
        CGame & MovePlayer ( CPlayer & player, const EMove & move );

        CGame & PlaceBomb ( CPlayer & player, const CCoord & coord );

        CGame & BombTick ();

        bool ValidMove ( const CPlayer & player, const EMove & move ) const;

        int PlayersAlive ( void ) const;

        void PrintResult () const;


};