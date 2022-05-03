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
class CMap 
{
    public:
        std::vector<std::vector<char>> m_Map;

        CMap ( std::string filename );

        CMap & Build ();

        CMap & InsertPlayer ( char num, CCoord cord );

        CMap & PlaceBomb ( const CCoord & c );
        
        void Print () const;
        
        bool IsFree ( const CCoord & c ) const;

        size_t GetWidth () const  { return m_Width; }

        size_t GetHeight () const { return m_Height; }

    private:
        size_t m_Width;
        size_t m_Height;
        std::string m_Filename;

        void PrintEdge () const;
};