#include "game.h"


using namespace std;

int getRandomAction(int min, int max) {
    return rand() % (max - min + 1) + min;
}

int checkwin(int *state, int player) {
    if (state[0] == player && state[1] == player && state[2] == player)
        return 1;
    else if (state[3] == player && state[4] == player && state[5] == player)
        return 1;
    else if (state[6] == player && state[7] == player && state[8] == player)
        return 1;
    else if (state[0] == player && state[3] == player && state[6] == player)
        return 1;
    else if (state[1] == player && state[4] == player && state[7] == player)
        return 1;
    else if (state[2] == player && state[5] == player && state[8] == player)
        return 1;
    else if (state[0] == player && state[4] == player && state[8] == player)
        return 1;
    else if (state[2] == player && state[4] == player && state[6] == player)
        return 1;
    return 0;
}

void copystate(struct board *current, int *state) {
    for (int i = 0; i < 9; i++)
        state[i] = current->state[i];
}



int filled(int *state) {
    for (int i = 0; i < 9; i++) {
        if (state[i] == -1)
            return 0;
    }
    return 1;
}

void printstate(int *state) {
    for (int i = 0; i < 3; i++) {
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

    for (int i = 3; i < 6; i++) {
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

    for (int i = 6; i < 9; i++) {
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

int minimax(int *state, bool isMaximizing) {
    if (checkwin(state, 1))
        return 10;
    if (checkwin(state, 0))
        return -10;
    if (filled(state))
        return 0;

    if (isMaximizing) {
        int bestVal = -1000;
        for (int i = 0; i < 9; i++) {
            if (state[i] == -1) {
                state[i] = 1;
                int value = minimax(state, false);
                state[i] = -1;
                bestVal = max(bestVal, value);
            }
        }
        return bestVal;
    } else {
        int bestVal = 1000;
        for (int i = 0; i < 9; i++) {
            if (state[i] == -1) {
                state[i] = 0;
                int value = minimax(state, true);
                state[i] = -1;
                bestVal = min(bestVal, value);
            }
        }
        return bestVal;
    }
}

int getMinimaxAction(int *state, double epsilon) {
    if ((double)rand() / RAND_MAX < epsilon) {
        int action;
        do {
            action = getRandomAction(0, 8);
        } while (state[action] != -1);
        return action;
    } else {
        int bestVal = 1000;
        int bestMove = -1;
        for (int i = 0; i < 9; i++) {
            if (state[i] == -1) {
                state[i] = 0;
                int moveVal = minimax(state, true);
                state[i] = -1;
                if (moveVal < bestVal) {
                    bestMove = i;
                    bestVal = moveVal;
                }
            }
        }
        return bestMove;
    }
}

double exec(struct board *current, struct board *start, int &result) {
    int *state = new int[9];
    copystate(current, state);
    printstate(state);
    while (true) {
        int act = getaction(current, epsilon);
        if (state[act] != -1) {
            updateQsa(current, act, -1);
            result = -1;
            return -2;
        }
        state[act] = 1;
        printstate(state);

        if (checkwin(state, 1)) {
            updateQsa(current, act, 1);
            result = 1;
            return 1;
        }
        if (filled(state)) {
            result = -1;
            return -2;
        }

        int pact = getMinimaxAction(state, epsilon);
        cout << "BOT action: " << pact << "\n";
        state[pact] = 0;
        printstate(state);

        if (checkwin(state, 0)) {
            updateQsa(current, act, -1);
            result = 0;
            return -1;
        }
        if (filled(state)) {
            result = -1;
            return -2;
        }
        current = search(start, state);
    }
}