/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#include "CInterface.h"

using namespace std;

void CInterface::PrintMenu()
{
    system( "clear" );
    cout << "``````````BOMBERMAN````````````" << endl;
    cout << "Press 0 for game rules." << endl;
    cout << "Press 1 for 2 player game." << endl;
    cout << "Press 2 to play against computer." << endl;
    cin >> menu_choice;
    while ( menu_choice < '0' || '2' < menu_choice )
    {
        cout << "Invalid choice, try again." << endl;
        cin >> menu_choice;
    }
    switch ( menu_choice )
    {
        case( '0' ):
        {
            PrintHelp();
            break;
        }
        case( '1' ):
        {
            ChooseMap();
            break;
        }
        case( '2' ):
        {
            ChooseMap();
            break;
        }
        default:
            cout << "Wrong option!" << endl; 
    }
}

void CInterface::PrintHelp()
{
    system( "clear" );
    cout << "``````````BOMBERMAN````````````" << endl;
    cout << "Moves: " << endl;
    cout << "'u' .. up" << endl;
    cout << "'d' .. down" << endl;
    cout << "'l' .. left" << endl;
    cout << "'r' .. right" << endl;
    cout << "'b' .. place bomb" << endl;
    cout << "Press 1 for 2 player game." << endl;
    cout << "Press 2 to play against computer." << endl;
}

void CInterface::ChooseMap()
{
    system( "clear" );
    cout << "``````````BOMBERMAN````````````" << endl;
    cout << "Choose you map ( 0 .. 3 ) from smallest to biggest." << endl;
    cin >> map_choice;
    if ( map_choice < '0' || map_choice > '3' )
        throw std::invalid_argument( "Invalid map choice!" );
}