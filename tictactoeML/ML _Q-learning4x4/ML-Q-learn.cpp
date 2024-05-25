#include "ML-Q-learn.h"

int getmax(double *Qsa)
{
    int max = 0;
    for (int i = 0; i < SPACE_SIZE; i++)
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

    newb->Qsa = new double[SPACE_SIZE];
    for (int i = 0; i < SPACE_SIZE; i++)
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
    for (int i = 0; i < SPACE_SIZE; i++)
    {
        if (b1->state[i] != state[i])
            return 0;
    }

    return 1;
}

struct board *search(struct board *start, int *state)
{
    struct board *prev;
    while (start != 0)
    {
        if (samestate(start, state))
            return start;

        prev = start;
        start = start->next;
    }

    struct board *newb = newBoard(state);
    prev->next = newb;

    return newb;
}

// exploit with Pr 0.9
// explore with Pr 0.1

int getaction(struct board *st)
{
    // get random value between 0 and 1
    srand((unsigned)time(NULL));
    double r = ((double)rand() / (double)RAND_MAX);

    int action;

    if (0 <= r && r < 0.9) // adjust!!
        action = getmax(st->Qsa);
    else
        action = rand() % SPACE_SIZE;

    return action;
}

void copystate(struct board *current, int *state)
{
    // adjust update scheme!
    for (int i = 0; i < SPACE_SIZE; i++)
        state[i] = current->state[i];
}

void updateQsa(struct board *current, int act, double val)
{
    current->Qsa[act] = current->Qsa[act] + val;
}

struct board *reconstruct(const char *s)
{
    ifstream file;
    file.open(s);
    //std::cout << "file checkout!\n";
    if (!file.is_open()) {std::cout << "file empty!\n"; exit(1);}
    struct board *start = new struct board;
    struct board *iter = start;
    struct board *prev;

    while (!file.eof())
    {
        int *state = new int[SPACE_SIZE];
        for (int i = 0; i < SPACE_SIZE; i++)
        {
            int read;
            file >> read;
            state[i] = read;
        }

        double *Qsa = new double[SPACE_SIZE];
        for (int i = 0; i < SPACE_SIZE; i++)
        {
            double read;
            file >> read;
            Qsa[i] = read;
        }

        struct board *x = new struct board;
        x->state = state;
        x->Qsa = Qsa;

        iter->next = x;
        prev = iter;
        iter = iter->next;
    }

    prev->next = 0;
    file.close();

    return start->next;
}

void write(const char *s, struct board *start)
{
    fstream file;
    file.open(s);

    while (start != 0)
    {
        for (int i = 0; i < SPACE_SIZE; i++)
            file << start->state[i] << " ";

        for (int i = 0; i < SPACE_SIZE; i++)
            file << start->Qsa[i] << " ";

        file << "\n";

        start = start->next;
    }

    file.close();
}