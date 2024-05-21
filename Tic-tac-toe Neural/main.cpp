#include "game.h"

int main()
{
    srand(time(NULL));

    myneural::NeuralNetwork network(9, 128, 9, 0.3);

    std::vector<std::vector<double>> inputs;
    std::vector<double> targets;
    network.loadDataset("datasetTTT.txt", inputs, targets);

    double lambda = 0.01; // Коэффициент регуляризации
    for (int i = 0; i < inputs.size(); ++i)
    {
        network.train(inputs[i], targets[i], lambda);
    }

    double accuracy = network.calculateAccuracy(inputs, targets);
    std::cout << "Neural network accuracy: " << accuracy * 100 << "%\n";
    int win{0}, loss{0}, draw{0};
    for (size_t i = 0; i < 50; i++)
    {
        int result = game(network);
        switch (result)
        {
        case 1:
            win++;
            break;
        case 0:
            loss++;
            break;
        case -1:
            draw++;
            break;
        default:
            break;
        }
    }
    std::cout << "AI Win: " << win << "\nAI loss: " << loss << "\nDRAW: " << draw << std::endl;
    /*std::vector<double> boardState = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

    int bestMove = getBestMove(boardState, network);
    std::cout << "Best move: " << bestMove << std::endl;

    boardState = {1, -1, -1, -1, 0, -1, -1, -1, -1};

    bestMove = getBestMove(boardState, network);
    std::cout << "Best move: " << bestMove << std::endl;

    boardState = {1, 1, -1, -1, 0, -1, -1, 0, -1};

    bestMove = getBestMove(boardState, network);
    std::cout << "Best move: " << bestMove << std::endl;
    */
    return 0;
}