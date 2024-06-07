#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <ctime>


//#include <unistd.h>

struct board {
    int *state;
    double *Qsa;
    struct board *next;
};

double epsilon = 0.2;

int getmax(double *Qsa);
struct board *newBoard(int *state, double *Qsa);
int samestate(struct board *b1, int *state);
struct board *search(struct board *start, int *state);
int getaction(struct board *state, double epsilon);
void updateQsa(struct board *current, int act, double val);
struct board *reconstruct(const char *s);
void writetofile(const char *s, struct board *start);