#include"NeuralNetwork.h"


void printBoard(std::vector<double> board);
int makeTurnAI(myneural::NeuralNetwork network, std::vector<double> board);
int makeTurnBot(bool bot);
int checkWin(std::vector<double> board);
bool checkDraw(std::vector<double> board);
int game(myneural::NeuralNetwork network);

