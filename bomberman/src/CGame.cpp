/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#include "CGame.h"
#include <memory>
#include <iostream>

using namespace std;

CGame::CGame ( CMap map ) 
: m_Map ( map )
{}

CGame & CGame::AddPlayer ( unique_ptr<CPlayer> player )
{
    m_Players.push_back( move( player ) );
    return *this;
}

CGame & CGame::Run ()
{
    Init();
    m_Map.Print();
    while ( m_Alive > 1 )
    {
        for ( auto & player : m_Players )
        {
            EMove move = player->GetMove( m_Map );
            MovePlayer( *player, move );
        }
        for ( auto it = m_Bombs.begin(); it != m_Bombs.end(); ++it )
        {
            if ( --it->m_Time == 0 )
                Boom( it->m_Coord );
        }
        system( "clear" );
        m_Map.Print();    
    }
    PrintResult();
    return *this;
}

CGame & CGame::Init ()
{
    CPlayerHuman new_player(++m_Alive, m_Map.GetWidth());
    CPlayerHuman new_player2(++m_Alive, m_Map.GetWidth());
    m_Players.push_back( new_player.Clone() );
    m_Players.push_back( new_player2.Clone() );

    for ( auto & player : m_Players )
        m_Map.InsertPlayer( player->GetNum(), player->GetCoord() );
    return *this;
}

CGame & CGame::MovePlayer   ( CPlayer & player, const EMove & move )
{
    if ( !ValidMove( player, move) )
        return *this;
    switch ( move )
    {
        case EMove::up:
        {
            m_Map.m_Map[player.m_Coord.m_X][player.m_Coord.m_Y] = ' ';
            m_Map.m_Map[--player.m_Coord.m_X][player.m_Coord.m_Y] = player.GetNum();
            player.m_Direction = EMove::up;
            break;            
        }
        case EMove::down:
        {
            m_Map.m_Map[player.m_Coord.m_X][player.m_Coord.m_Y] = ' ';
            m_Map.m_Map[++player.m_Coord.m_X][player.m_Coord.m_Y] = player.GetNum();
            player.m_Direction = EMove::down;
            break;  
        }
        case EMove::left:
        {
            m_Map.m_Map[player.m_Coord.m_X][player.m_Coord.m_Y] = ' ';
            m_Map.m_Map[player.m_Coord.m_X][--player.m_Coord.m_Y] = player.GetNum();
            player.m_Direction = EMove::left;
            break; 
        }
        case EMove::right:
        {
            m_Map.m_Map[player.m_Coord.m_X][player.m_Coord.m_Y] = ' ';
            m_Map.m_Map[player.m_Coord.m_X][++player.m_Coord.m_Y] = player.GetNum();
            player.m_Direction = EMove::right;
            break; 
        }
        case EMove::bomb:
        {
            switch ( player.m_Direction )
            {
                case EMove::up:
                {
                    PlaceBomb( CCoord( player.m_Coord.m_X - 1, player.m_Coord.m_Y ));
                    break;                    
                }
                case EMove::down:
                {
                    PlaceBomb( CCoord( player.m_Coord.m_X + 1, player.m_Coord.m_Y ));
                    break;                    
                }
                case EMove::left:
                {
                    PlaceBomb( CCoord( player.m_Coord.m_X, player.m_Coord.m_Y - 1 ));
                    break;                    
                }
                case EMove::right:
                {
                    PlaceBomb( CCoord( player.m_Coord.m_X, player.m_Coord.m_Y + 1 ));
                    break;                    
                }
                case EMove::bomb:
                {
                    cout << "Error (direction of a player == bomb)" << endl;
                    throw std::exception();                    
                }
            }
        }
    }
    return *this;        
}

bool CGame::ValidMove ( const CPlayer & player, const EMove & move ) const
{
    // Move out of map
    if ( player.m_Coord.m_X == 0 && move == EMove::up )
        return false;
    if ( player.m_Coord.m_X == m_Map.GetHeight() - 1 && move == EMove::down )
        return false;
    if ( player.m_Coord.m_Y == 0 && move == EMove::left )
        return false;
    if ( player.m_Coord.m_Y == m_Map.GetWidth() - 1 && move == EMove::right )
        return false;
    // Move on object
    if ( move == EMove::up && !m_Map.IsFree( CCoord( player.m_Coord.m_X - 1, player.m_Coord.m_Y) ))
        return false;
    if ( move == EMove::down && !m_Map.IsFree( CCoord( player.m_Coord.m_X + 1, player.m_Coord.m_Y) ))
        return false;
    if ( move == EMove::left && !m_Map.IsFree( CCoord( player.m_Coord.m_X, player.m_Coord.m_Y - 1) ))
        return false;
    if ( move == EMove::right && !m_Map.IsFree( CCoord( player.m_Coord.m_X, player.m_Coord.m_Y + 1) ))
        return false;
    // Bomb placement
    if ( move == EMove::bomb )
    {
        // Bomb out of map
        if ( player.m_Coord.m_X == 0 && player.m_Direction == EMove::up )
            return false;
        if ( player.m_Coord.m_X ==  m_Map.GetHeight() - 1 && player.m_Direction == EMove::down )
            return false;
        if ( player.m_Coord.m_Y == 0 && player.m_Direction == EMove::left )
            return false;
        if ( player.m_Coord.m_Y == m_Map.GetWidth() - 1 && player.m_Direction == EMove::right )
            return false;
        // Bomb on object
        if ( player.m_Direction == EMove::up && !m_Map.IsFree( CCoord( player.m_Coord.m_X - 1, player.m_Coord.m_Y) ))
            return false;
        if ( player.m_Direction == EMove::down && !m_Map.IsFree( CCoord( player.m_Coord.m_X + 1, player.m_Coord.m_Y) ))
            return false;
        if ( player.m_Direction == EMove::left && !m_Map.IsFree( CCoord( player.m_Coord.m_X, player.m_Coord.m_Y - 1) ))
            return false;
        if ( player.m_Direction == EMove::right && !m_Map.IsFree( CCoord( player.m_Coord.m_X, player.m_Coord.m_Y + 1) ))
            return false;
    } 
    return true;
}

CGame & CGame::PlaceBomb ( const CCoord & c )
{
    m_Map.PlaceBomb( CCoord( c.m_X, c.m_Y ));
    m_Bombs.push_back( CBomb( c ) );
    return *this;
}

CGame & CGame::Boom ( const CCoord & c )
{
    m_Map.m_Map[c.m_X][c.m_Y] = ' ';
    for ( int i = (int)c.m_X - 1; i < (int)c.m_X + 2 ; ++i )
        for ( int j = (int)c.m_Y - 1; j < (int)c.m_Y + 2; ++j )
            if ( i >= 0 &&  j >= 0 && i < (int)m_Map.GetHeight() && j < (int)m_Map.GetWidth() )
            {
                if ( m_Map.m_Map[i][j] == 'X' || m_Map.m_Map[i][j] == ' ' )
                    continue;
                if ( m_Map.m_Map[i][j] == '#' || m_Map.m_Map[i][j] == '*' )
                {
                    m_Map.m_Map[i][j] = ' ';
                    continue;
                }
                // Player died
                for ( auto it = m_Players.begin(); it != m_Players.end(); ++it )
                    if ( (*it)->GetNum() == m_Map.m_Map[i][j] )
                    {
                        m_Players.erase( it );
                        break;
                    }
                --m_Alive;
                m_Map.m_Map[i][j] = ' ';                
            }
    return *this;
}

void CGame::PrintResult() const
{
    if ( m_Players.empty() )
    {
        cout << "Draw!" << endl;
        return;
    }
    cout << "Player " << m_Players.front()->GetNum() << " wins!" << endl;
    return;
} 