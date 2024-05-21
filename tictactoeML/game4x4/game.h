#include "../ML _Q-learning4x4/ML-Q-learn.h"

int checkwin(int *state, int player);
void printstate(int *state);
int filled(int *state);
double exec(struct board *current, struct board *start);
void game(const char *QtableFilename);