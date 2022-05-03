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

class CPlayer
{
    public:
        CCoord m_Coord;
        EMove  m_Direction;

        CPlayer () = default;

        CPlayer ( size_t num, size_t map_size );

        CPlayer ( const CPlayer & ) = default;

        CPlayer & operator= ( const CPlayer & ) = delete;

        virtual ~CPlayer () = default;

        virtual std::unique_ptr<CPlayer> Clone () const = 0;

        virtual EMove GetMove ( const CMap & map ) = 0;
        
        CCoord GetCoord () const;

        char GetNum() const;

    protected:
        size_t m_Score;
        size_t m_Num;
        bool   m_Alive;
};