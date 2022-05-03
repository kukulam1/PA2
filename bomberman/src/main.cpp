/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#include <iostream>
#include "CMap.h"
#include "CGame.h"

using namespace std;


int main ( void )
{

    CMap mp ( "../maps/01.txt");
    mp.Build();

    CGame game ( mp );
    game.Run();

    cout << "Hello world!" << endl;
    return 0;
}
