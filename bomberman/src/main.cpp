/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#include <iostream>
#include "CMap.h"
#include "CGame.h"
#include "CInterface.h"

using namespace std;


int main ( void )
{

    CInterface interface;
    try 
    {
        interface.PrintMenu();
    }
    catch ( const invalid_argument & e )
    {
        std::cerr << e.what() << endl;
        return -1;
    }

    string map_name = string("examples/maps/0") + interface.map_choice + string(".txt");
    cout << map_name;

    CMap map ( map_name );

    try
    {
        map.Build();
    }
    catch( const std::exception & e )
    {
        std::cerr << e.what() << endl;
        return -1;
    }

    CGame game ( map, interface.menu_choice );
    game.Run();

    return 0;
}


