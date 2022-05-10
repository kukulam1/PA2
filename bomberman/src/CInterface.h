/**
 * @author Matej Kukula (kukulmat@fit.cvut.cz)
 * @date 2022-05-02
 */

#include <iostream>

class CInterface
{
    public:
        char menu_choice;
        char map_choice;

        void PrintMenu ();

        void PrintHelp ();

        void ChooseMap ();
};

