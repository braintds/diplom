#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#define SPACE_SIZE 16
#define ROWS_COLOMNS_COUNT 4
using namespace std;

struct board
{
    // array of size 9 representing each block on the board
    //-1 means empty, 0 means 'O' and 1 means 'X'
    int *state;
    double *Qsa;
    struct board *next;
};

int getmax(double *Qsa);
struct board *newBoard(int *state);
int samestate(struct board *b1, int *state);
struct board *search(struct board *start, int *state);
int getaction(struct board *st);
void copystate(struct board *current, int *state);
void updateQsa(struct board *current, int act, double val);
void write(const char *s, struct board *start);
struct board *reconstruct(const char *s);