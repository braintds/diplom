#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>
using namespace std;

struct board
{
    // array of size 9 representing each block on the board
    //-1 means empty, 0 means 'O' and 1 means 'X'
    int *state;
    double *Qsa;
    struct board *next;
};

bool isFree(int *state, int pos)
{
    if (pos >= 0 && pos < 9 && state[pos] == -1)
        return true;
    return false;
}

int getRandomState(int *state){
    int j{0};
    int *freeState = new int[9];
    for (int i{0}; i < 9; i++)
    {
        if (isFree(state, i)){
            freeState[j] = i;
            j++;
        }
    }
   if (j == 0) return -1; // Если нет свободных позиций
    int random = rand() % (j); // Используем j вместо j+1
    int result = freeState[random];
    delete[] freeState; // Освобождаем память
    return result;
}

// #######################################################
int getmax(double *Qsa)
{
    int max = 0;
    for (int i = 0; i < 9; i++)
    {
        if (Qsa[i] > Qsa[max])
            max = i;
    }
    return max;
}

struct board *newBoard(int *state)
{
    struct board *newb = new struct board;

    newb->state = state;

    newb->Qsa = new double[9];
    for (int i = 0; i < 9; i++)
    {
        if (state[i] == -1)
            newb->Qsa[i] = 0;
        else // assign large -ve value to filled position
            newb->Qsa[i] = -100;
    }

    newb->next = 0;
    return newb;
}

int samestate(struct board *b1, int *state)
{
    for (int i = 0; i < 9; i++)
    {
        if (b1->state[i] != state[i])
            return 0;
    }

    return 1;
}

struct board *search(struct board *start, int *state)
{ 
   struct board *prev = nullptr;
    struct board *current = start;
    while (current != nullptr)
    {
        if (samestate(current, state))
            return current;

        prev = current;
        current = current->next;
    }

    /*cout << endl;
    cout << "new!!" << endl;
    cout << endl;*/
    struct board *newb = newBoard(state);
    if (prev) 
        prev->next = newb; 
    else 
        start = newb;
}

// exploit with Pr 0.9
// explore with Pr 0.1

int getaction(struct board *st)
{
    // get random value between 0 and 1
    // srand((unsigned)time(NULL));
    double r = ((double)rand() / (double)RAND_MAX);

    int action;

    if (0 <= r && r < 0.9) // adjust!!
        action = getmax(st->Qsa);
    else
        action = rand() % 9;

    //cout << "action is: " << action << endl;

    return action;
}

// player: 1 indicates 'X', 0 indicates 'O'
// function: 0 indicates continue, 1 indicates win
int checkwin(int *state, int player)
{
    // check rows
    if (state[0] == player && state[1] == player && state[2] == player)
        return 1;

    else if (state[3] == player && state[4] == player && state[5] == player)
        return 1;

    else if (state[6] == player && state[7] == player && state[8] == player)
        return 1;

    // check columns

    else if (state[0] == player && state[3] == player && state[6] == player)
        return 1;

    else if (state[1] == player && state[4] == player && state[7] == player)
        return 1;

    else if (state[2] == player && state[5] == player && state[8] == player)
        return 1;

    // check diagonals

    else if (state[0] == player && state[4] == player && state[8] == player)
        return 1;

    else if (state[2] == player && state[4] == player && state[6] == player)
        return 1;

    return 0;
}

void copystate(struct board *current, int *state)
{

    // adjust update scheme!

    for (int i = 0; i < 9; i++)
        state[i] = current->state[i];
}

void updateQsa(struct board *current, int act, double val)
{
    current->Qsa[act] = current->Qsa[act] + val;
}

int filled(int *state)
{
    for (int i = 0; i < 9; i++)
    {
        if (state[i] == -1)
            return 0;
    }

    return 1;
}

void printstate(int *state)
{
    for (int i = 0; i < 3; i++)
    {
        if (state[i] == 1)
            cout << "X"
                 << " ";
        if (state[i] == 0)
            cout << "O"
                 << " ";
        if (state[i] == -1)
            cout << "-"
                 << " ";
    }
    cout << endl;

    for (int i = 3; i < 6; i++)
    {
        if (state[i] == 1)
            cout << "X"
                 << " ";
        if (state[i] == 0)
            cout << "O"
                 << " ";
        if (state[i] == -1)
            cout << "-"
                 << " ";
    }
    cout << endl;

    for (int i = 6; i < 9; i++)
    {
        if (state[i] == 1)
            cout << "X"
                 << " ";
        if (state[i] == 0)
            cout << "O"
                 << " ";
        if (state[i] == -1)
            cout << "-"
                 << " ";
    }
    cout << endl;
}

// computer always starts and is always 'X'
// returns 1 if computer wins. 0 if player wins
double exec(struct board *current, struct board *start)
{
    // printstate(current->state);

    int act = getaction(current);

    // if act is filled state, assumed as loss
    if (current->state[act] != -1)
    {
        updateQsa(current, act, -1);
        return -1;
    }

    int *state = new int[9];
    copystate(current, state);

    state[act] = 1;

    // printstate(state);

    if (checkwin(state, 1))
    {
        updateQsa(current, act, 1);
        return 1;
    }

    if (filled(state))
        return 0;

    int pact = getRandomState(state);
    if (pact == -1)
    {
        pact = getRandomState(state);
    }
   // cout << "BOT action: " << pact << "\n";

    // cin>>pact;

    state[pact] = 0;

    // printstate(state);

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
    if(val == 0)
    {
        return 0;
    }
}

struct board *reconstruct(const char *s)
{
    ifstream file;
    file.open(s);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return nullptr;
    }

    struct board *start = new struct board;
    struct board *iter = start;
    struct board *prev;

    while (!file.eof())
    {
        int *state = new int[9];
        for (int i = 0; i < 9; i++)
        {
            int read;
            file >> read;
            state[i] = read;
        }

        double *Qsa = new double[9];
        for (int i = 0; i < 9; i++)
        {
            double read;
            file >> read;
            Qsa[i] = read;
        }

         struct board *x = new struct board;
        x->state = state;
        x->Qsa = Qsa;
        x->next = nullptr;

        if (prev == nullptr) {
            start = x;
        } else {
            prev->next = x;
        }
        prev = x;
    }

    file.close();

    return start;
    }

void write(const char *s, struct board *start)
{
    fstream file;
    file.open(s);

    while (start != 0)
    {
        for (int i = 0; i < 9; i++)
            file << start->state[i] << " ";

        for (int i = 0; i < 9; i++)
            file << start->Qsa[i] << " ";

        file << endl;

        start = start->next;
    }

    file.close();
}

int main()
{
    srand((unsigned)time(NULL));
    const char *s = "table.txt";
    struct board *start = reconstruct(s);
if (start == nullptr) {
        cerr << "Error reconstructing board!" << endl;
        return -1;
    }
    int countModel = 0, countBot = 0, countDraws = 0;

    int *startstate = new int[9];
    startstate[0] = -1;
    startstate[1] = -1;
    startstate[2] = -1;
    startstate[3] = -1;
    startstate[4] = -1;
    startstate[5] = -1;
    startstate[6] = -1;
    startstate[7] = -1;
    startstate[8] = -1;

    struct board *newgame = search(start, startstate);
    for (int i = 0; i < 1000000; i++)
    {
        startstate = new int[9];
        startstate[0] = -1;
        startstate[1] = -1;
        startstate[2] = -1;
        startstate[3] = -1;
        startstate[4] = -1;
        startstate[5] = -1;
        startstate[6] = -1;
        startstate[7] = -1;
        startstate[8] = -1;

        // cout << "Part No " << i + 1 << "\n";
        double val = exec(newgame, start);

        if (val < 0)
        {
            // cout << "You win!!!" << endl;
            countBot++;
        }
        else if (val > 0)
        {
            //          cout << "You lose!!! :(" << endl;
            countModel++;
        }
        else
        {
            //       cout << "You lose!!! :(" << endl;
            countDraws++;
        }

        // usleep(2500);
        //  filled(newgame->state);
    }

    write(s, start);
    cout << "Model Wins: " << countModel << "\n";
    cout << "Bot Wins: " << countBot << "\n";
    cout << "Draws: " << countDraws << "\n";
    return 0;
}