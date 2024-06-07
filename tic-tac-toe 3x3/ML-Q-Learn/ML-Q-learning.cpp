#include "ML-Q-learning.h"

int getRandomFreeState(int *state){
    int* freeState = new int[9];
    size_t j{0};
    for (size_t i{0}; i<9; i++)
        if (state[i]==-1) {
            freeState[j] = i;
            j++;
        }
        return rand() % j + 1;
    }

int getmax(double *Qsa) {
    int max = 0;
    for (int i = 0; i < 9; i++) {
        if (Qsa[i] > Qsa[max])
            max = i;
    }
    return max;
}

struct board *newBoard(int *state, double *Qsa) {
    struct board *newb = new struct board;
    newb->state = new int[9];
    for (int i = 0; i < 9; i++) {
        newb->state[i] = state[i];
    }
    newb->Qsa = new double[9];
    for (int i = 0; i < 9; i++) {
        newb->Qsa[i] = Qsa[i];
    }
    newb->next = nullptr;
    return newb;
}


int samestate(struct board *b1, int *state) {
    for (int i = 0; i < 9; i++) {
        if (b1->state[i] != state[i])
            return 0;
    }
    return 1;
}

struct board *search(struct board *start, int *state) {
    struct board *prev;
    while (start != 0) {
        if (samestate(start, state))
            return start;
        prev = start;
        start = start->next;
    }

    double *Qsa = new double[9];
    for (int i = 0; i < 9; i++) {
        if (state[i] == -1)
            Qsa[i] = 0;
        else
            Qsa[i] = -100;
    }

    struct board *newb = newBoard(state, Qsa);
    prev->next = newb;
    return newb;
}

// exploit with Pr 0.9
// explore with Pr 0.1

int getaction(struct board *state, double epsilon) {
    if ((double)rand() / RAND_MAX < epsilon) {
        return getRandomFreeState(state->state);
    } else {
        return getmax(state->Qsa);
    }
}

void updateQsa(struct board *current, int act, double val) {
    current->Qsa[act] = current->Qsa[act] + val;
}

struct board *reconstruct(const char *s) {
    std::ifstream file(s);
    if (!file.is_open()) {
        std::cout << "Unable to open file!" << std::endl;
        return nullptr;
    }

    struct board *start = nullptr;
    struct board *prev = nullptr;

    while (!file.eof()) {
        int *state = new int[9];
        for (int i = 0; i < 9; i++) {
            int read;
            file >> read;
            state[i] = read;
        }

        double *Qsa = new double[9];
        for (int i = 0; i < 9; i++) {
            double read;
            file >> read;
            Qsa[i] = read;
        }

        struct board *newb = newBoard(state, Qsa);
        if (start == nullptr) {
            start = newb;
        } else {
            prev->next = newb;
        }
        prev = newb;
    }

    file.close();
    return start;
}

void writetofile(const char *s, struct board *start) {
    std::ofstream file(s);
    if (!file.is_open()) {
        std::cout << "Unable to open file!" << std::endl;
        return;
    }

    while (start != nullptr) {
        for (int i = 0; i < 9; i++) {
            file << start->state[i] << " ";
        }
        for (int i = 0; i < 9; i++) {
            file << start->Qsa[i] << " ";
        }
        file << std::endl;
        start = start->next;
    }

    file.close();
}