/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "CMap.h"
#include "CPlayer.h"
#include "CPlayerHuman.h"
#include "CPlayerAI.h"
#include "CBomb.h"

/**
 * @brief class CGame implements most of the game 
 */
class CGame
{
    public:
        CGame ( CMap map, char game_type );

        CGame & Run ();

    private:
        CMap m_Map;
        std::vector<std::shared_ptr<CPlayer>> m_Players;
        std::vector<CBomb> m_Bombs;
        size_t m_Alive = 0;

        CGame & Init ();
        
        CGame & MovePlayer ( CPlayer & player, const EMove & move );

        CGame & PlaceBomb ( CPlayer & player, const CCoord & coord );

        CGame & BombTick ();

        bool ValidMove ( const CPlayer & player, const EMove & move ) const;

        int PlayersAlive ( void ) const;

        void CheckHighScore ( int player_num ) const;

        void PrintResult () const;
};