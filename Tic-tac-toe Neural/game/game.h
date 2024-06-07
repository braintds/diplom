#include "../NeuralNetwork/NeuralNetwork.h"

void printBoard(const std::vector<double>& board);
int makeTurnAI(NeuralNetwork& network, std::vector<double>& board, double learningRate);
int makeTurnBot(std::vector<double>& board);
int checkWin(const std::vector<double>& board);
bool checkDraw(const std::vector<double>& board);
int playGame(NeuralNetwork& neuralNetwork, double learningRate);
int filled(const std::vector<double>& board);
int minimax(std::vector<double> board, bool isMaximizing);
int getMinimaxAction(std::vector<double>& board);