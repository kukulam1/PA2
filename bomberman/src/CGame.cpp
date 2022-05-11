/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#include "CGame.h"

using namespace std;

/**
 * @brief Construct a new CGame::CGame object
 * Add players to game  
 * @param map 
 * @param game_type '1' -> 2 player game, 2 -> against AI
 */
CGame::CGame ( CMap map, char game_type ) 
: m_Map ( map )
{
    CPlayerHuman new_player( ++m_Alive, m_Map.GetWidth() );
    m_Players.push_back( new_player.Clone() );
    if ( game_type == '1' )
    {
        CPlayerHuman new_player2( ++m_Alive, m_Map.GetWidth() );
        m_Players.push_back( new_player2.Clone() );
        return;
    }    
    if ( game_type == '2' )
    {
        CPlayerAI new_player2( ++m_Alive, m_Map.GetWidth() );
        m_Players.push_back( new_player2.Clone() );
        return;
    }
    throw "Wrong menu_choice in CGame constructor.";
}

/**
 * @brief Game implementation.
 */
CGame & CGame::Run ()
{
    Init();
    m_Map.Print();

    while ( PlayersAlive() > 1 )
    {
        for ( auto & player : m_Players )
        {
            EMove move = player->GetMove( m_Map );
            MovePlayer( *player, move );
            player->m_Score++;
        }
        BombTick();
        system( "clear" );
        m_Map.Print();
    }

    PrintResult();
    return *this;
}

/**
 * @brief Initialize game, add players to map
 * @return CGame& 
 */
CGame & CGame::Init ()
{
    for ( auto & player : m_Players )
        m_Map.InsertPlayer( player->GetNum(), player->GetCoord() );
    return *this;
}

/**
 * @brief Player move.
 * @param player 
 * @param move 
 */
CGame & CGame::MovePlayer   ( CPlayer & player, const EMove & move )
{
    if ( !ValidMove( player, move) )
        return *this;
    switch ( move )
    {
        case EMove::up:
        {
            if ( player.m_Direction != EMove::up )
            {
                player.m_Direction = EMove::up;
                break;                
            }
            m_Map.m_Map[player.m_Coord.m_X][player.m_Coord.m_Y] = ' ';
            if ( m_Map.m_Map[--player.m_Coord.m_X][player.m_Coord.m_Y]  == '*' )
                player.GetPowerUp();
            m_Map.m_Map[player.m_Coord.m_X][player.m_Coord.m_Y] = player.GetNum();
            player.m_Direction = EMove::up;
            break;            
        }
        case EMove::down:
        {
            if ( player.m_Direction != EMove::down )
            {
                player.m_Direction = EMove::down;
                break;                
            }
            m_Map.m_Map[player.m_Coord.m_X][player.m_Coord.m_Y] = ' ';
            if ( m_Map.m_Map[++player.m_Coord.m_X][player.m_Coord.m_Y]  == '*' )
                player.GetPowerUp();
            m_Map.m_Map[player.m_Coord.m_X][player.m_Coord.m_Y] = player.GetNum();
            player.m_Direction = EMove::down;
            break;  
        }
        case EMove::left:
        {
            if ( player.m_Direction != EMove::left )
            {
                player.m_Direction = EMove::left;
                break;                
            }
            m_Map.m_Map[player.m_Coord.m_X][player.m_Coord.m_Y] = ' ';
            if ( m_Map.m_Map[player.m_Coord.m_X][--player.m_Coord.m_Y]  == '*' )
                player.GetPowerUp();
            m_Map.m_Map[player.m_Coord.m_X][player.m_Coord.m_Y] = player.GetNum();
            player.m_Direction = EMove::left;
            break; 
        }
        case EMove::right:
        {
            if ( player.m_Direction != EMove::right )
            {
                player.m_Direction = EMove::right;
                break;                
            }
            m_Map.m_Map[player.m_Coord.m_X][player.m_Coord.m_Y] = ' ';
            if ( m_Map.m_Map[player.m_Coord.m_X][++player.m_Coord.m_Y]  == '*' )
                player.GetPowerUp();
            m_Map.m_Map[player.m_Coord.m_X][player.m_Coord.m_Y] = player.GetNum();
            player.m_Direction = EMove::right;
            break; 
        }
        case EMove::bomb:
        {
            switch ( player.m_Direction )
            {
                case EMove::up:
                {
                    PlaceBomb( player, CCoord( player.m_Coord.m_X - 1, player.m_Coord.m_Y ));
                    break;                    
                }
                case EMove::down:
                {
                    PlaceBomb( player, CCoord( player.m_Coord.m_X + 1, player.m_Coord.m_Y ));
                    break;                    
                }
                case EMove::left:
                {
                    PlaceBomb( player, CCoord( player.m_Coord.m_X, player.m_Coord.m_Y - 1 ));
                    break;                    
                }
                case EMove::right:
                {
                    PlaceBomb( player, CCoord( player.m_Coord.m_X, player.m_Coord.m_Y + 1 ));
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

/**
 * @brief Checks if move is valid.
 * @param player 
 * @param move 
 */
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

CGame & CGame::PlaceBomb ( CPlayer & player, const CCoord & coord )
{
    player.m_Bomb->m_Coord = coord;
    m_Map.PlaceBomb( player.m_Bomb->GetVisual(), coord );
    m_Bombs.push_back( move( player.m_Bomb ) );
    player.m_Bomb = make_shared<CBomb>();
    return *this;
}
/**
 * @brief Print results of the game.
 */
void CGame::PrintResult() const
{
    for ( size_t i = 0; i < m_Map.GetHeight(); ++i )
        for ( size_t j = 0; j < m_Map.GetWidth(); ++j )
        {
            if ( '1' <= m_Map.m_Map[i][j] && m_Map.m_Map[i][j] <= '4' )
            {
                cout << "Player " << m_Map.m_Map[i][j] << " wins!" << endl;
                //int -> char: Players from 1, not from 0 -> - '1' 
                CheckHighScore( m_Map.m_Map[i][j] - '1' );
                return;
            }
        }
    cout << "Draw" << endl;
    return;
}
/**
 * @brief Check if new high score by human player
 * @param player_num 
 */
void CGame::CheckHighScore( int player_num ) const
{
    ifstream in_file( "score/high_score.txt" );
    size_t high_score;
    in_file >> high_score;
    in_file.close();
    //check if player with high score is human player
    if ( m_Players[ player_num ]->m_Score > high_score && 
         dynamic_pointer_cast<CPlayerHuman>(m_Players[ player_num ]) != nullptr )
    {
        ofstream out_file( "score/high_score.txt" );
        out_file << m_Players[ player_num ]->m_Score << endl;
        out_file.close();
        cout << "New high score " << m_Players[ player_num ]->m_Score << '!' << endl;
    }            
}

int CGame::PlayersAlive ( void ) const
{
    int alive = 0;
    for ( size_t i = 0; i < m_Map.GetHeight(); ++i )
        for ( size_t j = 0; j < m_Map.GetWidth(); ++j )
            if ( m_Map.m_Map[i][j] == '1' || m_Map.m_Map[i][j] == '2' )
                ++alive;
    return alive;
}


/**
 * @brief Bombs tick a those with no time left explode
 * 
 * @return CGame& 
 */
CGame & CGame::BombTick () 
{
    auto it = m_Bombs.begin();
    while ( it != m_Bombs.end() )
    {
        if ( --((*it)->m_Time) == 0 )
        {
            (*it)->Effect( m_Map );
            it = m_Bombs.erase( it );
        }
        else
            ++it;
    }
    return *this;
}