#include "game.h"

bool isFree(int *state, int pos)
{
    if (pos >= 0 && pos < SPACE_SIZE && state[pos] == -1)
        return true;
    return false;
}

int getRandomState(int *state){
    int j{0};
    int *freeState = new int[SPACE_SIZE];
    for (int i{0}; i < SPACE_SIZE; i++)
    {
        if (isFree(state, i)){
            freeState[j] = i;
            j++;
        }
    }
    int random = rand() % (j + 1);
    return freeState[random];
}

// player: 1 indicates 'X', 0 indicates 'O'
// function: 0 indicates continue, 1 indicates win
/*=================================================================================*/
int checkwin(int *state, int player)
{
    size_t i{0};
    if (state[0] == player && state[1] == player && state[2] == player && state[3] == player)
        return 1;

    else if (state[4] == player && state[5] == player && state[6] == player && state[7] == player)
        return 1;

    else if (state[8] == player && state[9] == player && state[10] == player && state[11] == player)
        return 1;
    else if (state[12] == player && state[13] == player && state[14] == player && state[15] == player)
        return 1;

    // check columns

    else if (state[0] == player && state[4] == player && state[8] == player && state[12] == player)
        return 1;

    else if (state[1] == player && state[5] == player && state[9] == player && state[13] == player)
        return 1;

    else if (state[2] == player && state[6] == player && state[10] == player && state[14] == player)
        return 1;
    else if (state[3] == player && state[7] == player && state[11] == player && state[15] == player)
        return 1;

    // check diagonals

    else if (state[0] == player && state[5] == player && state[10] == player && state[15] == player)
        return 1;

    else if (state[3] == player && state[6] == player && state[9] == player && state[12] == player)
        return 1;

    return 0;
    
    // // check rows
    // do
    // {
    //     // ====================================change for different area size====================================//
    //     if (((state[i] == state[i + 1]) && (state[i] == state[i + 2]) && (state[i] == state[i + 3])) && state[i] == player)
    //     {
    //         return 1;
    //     }
    //     i += ROWS_COLOMNS_COUNT;
    // } while (i < SPACE_SIZE);

    // // check columns
    // i = 0;
    // do
    // {
    //     if ((state[i] == state[i + ROWS_COLOMNS_COUNT] && state[i] == state[i + ROWS_COLOMNS_COUNT * 2] && state[i] == state[i + ROWS_COLOMNS_COUNT * 3]) && state[i] == player)
    //     {

    //         return 1;
    //     }
    //     i++;
    // } while (i < ROWS_COLOMNS_COUNT);

    // // check diagonals
    // // ====================================change for different area size====================================//

    // if (((state[0] == state[5] && state[0] == state[10] && state[0] == state[15]) ||
    //      (state[3] == state[6] && state[3] == state[9] && state[3] == state[12])) &&
    //     state[i] == player)
    // {
    //     return 1;
    // }

    return 0;
}

void newGame(int *board, int size)
{
    for (size_t i = 0; i < SPACE_SIZE; i++)
    {
        board[i] = -1;
    }
}

void printstate(int *state)
{
    std::cout << "\nNEW STATE\n";
    for (int i = 0; i < SPACE_SIZE; i++)
    {
        if (state[i] == -1)
            std::cout << "- ";
        if (state[i] == 1)
            std::cout << "X ";
        if (state[i] == 0)
            std::cout << "O ";
        if ((i+1)% ROWS_COLOMNS_COUNT == 0 && i != 0)
            std::cout << "\n";
    }
    std::cout << "\n";
}

int filled(int *state)
{
    for (int i = 0; i < SPACE_SIZE; i++)
    {
        if (state[i] == -1)
            return 0;
    }

    return 1;
}

// computer always starts and is always 'X'
// returns 1 if computer wins. 0 if player wins
double exec(struct board *current, struct board *start)
{
    //printstate(current->state);

    int act = getaction(current);

    // if act is filled state, assumed as loss
    if (current->state[act] != -1)
    {
        updateQsa(current, act, -1);
        return -1;
    }

    int *state = new int[SPACE_SIZE];
    copystate(current, state);

    state[act] = 1;

    //printstate(state);

    if (checkwin(state, 1))
    {
        updateQsa(current, act, 1);
        return 1;
    }

    if (filled(state))
        return 0;

    /*std::cout << "enter action: ";
    int pact{-1};
    do {
        cin >> pact;       
    }while (!isFree(state, pact));*/
    state[getRandomState(state)] = 0;

    //printstate(state);

    if (checkwin(state, 0))
    {
        updateQsa(current, act, -1);
        return -1;
    }

    struct board *newb = search(start, state);

    double val = exec(newb, start);
    if (val < 0)
    {
        updateQsa(current, act, 0.8 * val);
        return 0.8 * val;
    }
    else
    {
        updateQsa(current, act, 0.8 * val);
        return 0.8 * val;
    }
    if (val == 0){
        return 0;
    }
}

void game(const char *QtableFilename)
{
    srand((unsigned)time(NULL));
    int counter{0};
    int MLwins{0}, MLloose{0}, draws{0};
    while (counter < 50)
    {
        struct board *start = reconstruct(QtableFilename);

        int *startstate = new int[SPACE_SIZE];
        newGame(startstate, SPACE_SIZE);

        struct board *newgame = search(start, startstate);

        double val = exec(newgame, start);
        //std::cout << "\n\n\n";

        if (val < 0)
            MLloose++;
        else
            MLwins++;
        if (val == 0)
            draws++;
        //std::cout << endl;

        write(QtableFilename, start);
        counter++;
       // std::cout << "\n penis count "<< counter;
    }
    std::cout <<"Model wins: " << MLwins << " times" << "\nModel loose: " << MLloose << " times" << "\n Draws: " << draws<< "\n";
}
