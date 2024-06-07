#include "game/game.h"
#include "ML-Q-Learn/ML-Q-learning.h"

using namespace std;

int main() {
    srand(time(NULL));

    int episodes = 1000;
    double epsilon = 0.2; // Начальное значение эпсилона
    struct board *start = new board;
    start->state = new int[9];
    for (int i = 0; i < 9; i++) {
        start->state[i] = -1;
    }
    start->Qsa = new double[9];
    for (int i = 0; i < 9; i++) {
        start->Qsa[i] = 0;
    }
    start->next = nullptr;

    int xWins = 0;
    int oWins = 0;
    int draws = 0;

    for (int i = 0; i < episodes; i++) {
        cout << "\nEpisode: " << i + 1 << endl;
        struct board *current = start;
        int result = 0;
        exec(current, start, result);
         epsilon *= 0.9;

        if (result == 1) {
            xWins++;
            cout << "X wins" << endl;
        } else if (result == 0) {
            oWins++;
            cout << "O wins" << endl;
        } else {
            draws++;
            cout << "Draw" << endl;
        }
    }

    cout << "\nX wins: " << xWins << endl;
    cout << "O wins: " << oWins << endl;
    cout << "Draws: " << draws << endl;

    writetofile("output.txt", start);

    return 0;
}