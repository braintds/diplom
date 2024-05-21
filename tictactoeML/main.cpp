#include "game4x4/game.h"

int main()
{
    // reconstruct saved board states
    const char *s = "table4x4.txt";
    game(s);

    return 0;
}