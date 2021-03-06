/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include "CCoord.h"
#include "EMove.h"

/**
 * @brief class CMap implements map
 */
class CMap 
{
    public:
        std::vector<std::vector<char>> m_Map;

        CMap ( std::string filename );

        CMap & Build ();

        CMap & InsertPlayer ( char num, CCoord coord );

        CMap & PlaceBomb ( const char c, const CCoord & coord );

        CCoord FindPlayer ( const char player_num ) const;

        bool Explode ( const CCoord & c );

        bool WillExplodeClose ( const CCoord & c ) const;
        
        void Print () const;
        
        bool IsFree ( const CCoord & c ) const;

        bool IsPlayer ( const CCoord & c ) const;

        size_t GetWidth () const  { return m_Width; }

        size_t GetHeight () const { return m_Height; }

    private:
        size_t m_Width;
        size_t m_Height;
        std::string m_Filename;

        void PrintEdge () const;
};