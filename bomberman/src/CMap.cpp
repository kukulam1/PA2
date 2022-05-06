/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "CMap.h"

using namespace std;

CMap::CMap ( std::string filename )
: m_Filename ( filename )
{}

/**
 * @brief Loads map from file into m_Map
 * @return CMap& 
 */
CMap & CMap::Build ()
{
    try 
    {
        ifstream map_file;
        map_file.open( m_Filename );
        map_file >> m_Width;
        map_file >> m_Height;
        m_Map = std::vector<std::vector<char>>(m_Height, std::vector<char>(m_Width, ' '));
        string buffer;
        size_t line_num = 0;
        getline( map_file, buffer);
        while ( getline( map_file, buffer) )
        {
            for ( size_t i = 0; i < buffer.size() - 1; ++i )
                m_Map[line_num][i] = buffer[i];
            ++line_num;
        }
        map_file.close();
    }
    catch ( exception const & e )
    {
        cout << "Error with map loading!" << endl;
        throw e;
    }
    return *this;
}

CMap & CMap::InsertPlayer ( char num, CCoord cord )
{
    m_Map[cord.m_X][cord.m_Y] = num;
    return *this;
}


CMap & CMap::PlaceBomb ( const char c, const CCoord & coord )
{
    m_Map[coord.m_X][coord.m_Y] = c;
    return *this;
}

bool CMap::IsFree ( const CCoord & c ) const
{
    return ( m_Map[c.m_X][c.m_Y] == ' ' || m_Map[c.m_X][c.m_Y] == '*' );
}

void CMap::Print () const 
{
    PrintEdge();
    for ( size_t i = 0; i < m_Height; ++i )
    {
        cout << '|';
        for ( size_t j = 0; j < m_Width; ++j )
        {
            cout << m_Map[i][j];
        }
        cout << '|' << endl;
    }
    PrintEdge();
}

void CMap::PrintEdge () const 
{
    cout << '+';
    for ( size_t i = 0; i < m_Height; ++i )
        cout << '-';
    cout << '+' << endl;
}

bool CMap::Explode ( const CCoord & c )
{
    bool killed = false;
    if ( '1' <= m_Map[c.m_X][c.m_Y] && m_Map[c.m_X][c.m_Y] <=  '4' )
        killed = true;
    if ( m_Map[c.m_X][c.m_Y] != 'X' )
        m_Map[c.m_X][c.m_Y] = ' ';
    return killed;
}
