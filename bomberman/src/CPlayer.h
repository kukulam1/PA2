/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#pragma once

#include <cstddef>
#include <memory>
#include "EMove.h"
#include "CMap.h"
#include "CCoord.h"
#include "CBomb.h"
#include "CDoubleBomb.h"
#include "CFlameBomb.h"
#include "CAllDirectionBomb.h"

/**
 * @brief Abstract class player
 * Virtual method GetMove -> input from player or AI move  
 */
class CPlayer
{
    public:
        CCoord m_Coord;
        EMove  m_Direction;
        size_t m_Score;
        std::shared_ptr<CBomb> m_Bomb;

        CPlayer () = default;

        CPlayer ( size_t num, size_t map_size );

        CPlayer ( const CPlayer & c );

        CPlayer & operator= ( const CPlayer & ) = delete;

        virtual ~CPlayer () = default;

        virtual std::unique_ptr<CPlayer> Clone () const = 0;

        virtual EMove GetMove ( const CMap & map ) = 0;

        CPlayer & GetPowerUp ();
        
        CCoord GetCoord () const;

        char GetNum() const;

    protected:
        size_t m_Num;
        bool   m_Alive;
};