#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <ctime>
#include"../ML-Q-Learn/ML-Q-learning.h"

int getRandomAction(int min, int max);
int checkwin(int *state, int player);
void copystate(struct board *current, int *state);
int filled(int *state);
void printstate(int *state);
int minimax(int *state, bool isMaximizing);
int getMinimaxAction(int *state, double epsilon);
double exec(struct board *current, struct board *start, int &result);