#include "game.h"

// player: 1 indicates 'X', 0 indicates 'O'
// function: 0 indicates continue, 1 indicates win
/*=================================================================================*/
int checkwin(int *state, int player)
{
    size_t i{0};
    // check rows
    do
    {
        // ====================================change for different area size====================================//
        if ((state[i] == state[i + 1] && state[i] == state[i + 2] && state[i] == state[i + 3]) && (state[i] == 1 || state[i] == 0))
        {
            switch (state[i])
            {
            case 0:
                return 0;
            case 1:
                return 1;
            default:
                break;
            }
        }
        i += 3;
    } while (i < SPACE_SIZE);

    // check columns
    i = 0;
    do
    {
        if ((state[i] == state[i + ROWS_COLOMNS_COUNT] && state[i] == state[i + ROWS_COLOMNS_COUNT * 2] && state[i] == state[i + ROWS_COLOMNS_COUNT * 3]) && (state[i] == 1 || state[i] == 0))
        {
            switch ((int)state[i])
            {
            case 0:
                return 0;
            case 1:
                return 1;
            default:
                break;
            }
        }
        i++;
    } while (i < ROWS_COLOMNS_COUNT);

    // check diagonals
    // ====================================change for different area size====================================//

    if (((state[0] == state[5] && state[0] == state[10] && state[0] == state[15]) ||
         (state[3] == state[6] && state[3] == state[9] && state[3] == state[12])) &&
        (state[i] == 1 || state[i] == 0))
    {
        switch ((int)state[i])
        {
        case 0:
            return 0;
        case 1:
            return 1;
        default:
            break;
        }
    }

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
    for (int i = 0; i < SPACE_SIZE; i++)
    {
        if (state[i] == -1)
            cout << "- ";
        if (state[i] == 1)
            cout << "X ";
        if (state[i] == 0)
            cout << "O ";
        if (i % ROWS_COLOMNS_COUNT == 0)
            cout << "\n";
    }
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
    printstate(current->state);

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

    printstate(state);

    if (checkwin(state, 1))
    {
        updateQsa(current, act, 1);
        return 1;
    }

    if (filled(state))
        return 0;

    cout << "enter action: ";
    int pact;
    cin >> pact;

    state[pact] = 0;

    printstate(state);

    if (checkwin(state, 0))
    {
        updateQsa(current, act, -1);
        return -1;
    }

    struct board *newb = search(start, state);

    double val = exec(newb, start);
    if (val <= 0)
    {
        updateQsa(current, act, 0.8 * val);
        return 0.8 * val;
    }
    else
    {
        updateQsa(current, act, 0.8 * val);
        return 0.8 * val;
    }
}

void game(const char *QtableFilename)
{
    struct board *start = reconstruct(QtableFilename);

    int *startstate = new int[SPACE_SIZE];
    newGame(startstate, SPACE_SIZE);

    struct board *newgame = search(start, startstate);

    double val = exec(newgame, start);
    cout << "\n\n\n";

    if (val <= 0)
        cout << "You win!!!\n";
    else
        cout << "You lose!!!\n";

    cout << endl;

    write(QtableFilename, start);
}
