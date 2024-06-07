#include"game.h"


void printBoard(const std::vector<double>& board) {
    for (int i = 0; i < 9; i++) {
        if (board[i] == 1) std::cout << "X ";
        else if (board[i] == 0) std::cout << "O ";
        else std::cout << ". ";
        if ((i + 1) % 3 == 0) std::cout << std::endl;
    }
}

int makeTurnAI(NeuralNetwork& network, std::vector<double>& board, double learningRate) {
    network.forwardPropagation();
    std::vector<double> output = network.getOutput();
    int move = std::distance(output.begin(), std::max_element(output.begin(), output.end()));
    while (board[move] != -1) {
        output[move] = -1;
        move = std::distance(output.begin(), std::max_element(output.begin(), output.end()));
    }
    return move;

}

int makeTurnBot(std::vector<double>& board) {
    std::vector<int> availableMoves;
    for (int i = 0; i < 9; i++) {
        if (board[i] == -1) {
            availableMoves.push_back(i);
        }
    }
    int move = availableMoves[rand() % availableMoves.size()];
    board[move] = 0; // 'O' делает ход
    return move;
}

int checkWin(const std::vector<double>& board) {
    const int winPatterns[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };
    
    for (auto& pattern : winPatterns) {
        if (board[pattern[0]] == board[pattern[1]] && board[pattern[1]] == board[pattern[2]] && board[pattern[0]] != -1) {
            return board[pattern[0]] == 1 ? 1 : -1;
        }
    }
    return 0;
}

bool checkDraw(const std::vector<double>& board) {
    for (auto& cell : board) {
        if (cell == -1) return false;
    }
    return true;
}


int playGame(NeuralNetwork& neuralNetwork, double learningRate) {
    std::vector<double> board(inputSize, -1); // Инициализация пустой доски
    int turn = 1; // 1 для 'X', 0 для 'O'
    int moveCount = 0;

    while (true) {
        printBoard(board);

        int move;
        if (turn == 1) {
            move = makeTurnAI(neuralNetwork, board, learningRate);
            board[move] = 1;
        } else {
            std::vector<double> boardCopy(inputSize,-1);
            for(size_t i{0}; i<inputSize;i++){
                boardCopy[i] = board[i];
            }
            move = getMinimaxAction(boardCopy);
            board[move] = 0;
        }

        int winner = checkWin(board);
        if (winner != 0 || checkDraw(board))
        {
            //printBoard(board);
            //std::cout<<std::endl;
            if (winner == 1)
            {
                //std::cout << "X wins!" << std::endl;
                return 1;
            }
            else if (winner == -1)
            {
                //std::cout << "O wins!" << std::endl;
                return -1;
            }
            else
            {
                //std::cout << "It's a draw!" << std::endl;
                return 0;
            }
        }

        turn = 1 - turn; // Переключаем ход
        moveCount++;
    }
}

int filled(const std::vector<double>& board)
{
    for(int i=0;i<board.size();i++)
    {
        if(board[i]==-1)
            return 0;
    }

    return 1;
}

int minimax(std::vector<double> board, bool isMaximizing) {
    switch (checkWin(board))
    {
    case 1:
        return 10;
    case 2:
        return -10;
    default:
        break;
    }
    if (filled(board)) return 0;

    if (isMaximizing) {
        int bestVal = -1000;
        for (int i = 0; i < 9; i++) {
            if (board[i] == -1) {
                board[i] = 1;
                int value = minimax(board, false);
                board[i] = -1;
                //bestVal = max(bestVal, value);
                if (bestVal < value) bestVal = value;
            }
        }
        return bestVal;
    } else {
        int bestVal = 1000;
        for (int i = 0; i < 9; i++) {
            if (board[i] == -1) {
                board[i] = 0;
                int value = minimax(board, true);
                board[i] = -1;
                //bestVal = min(bestVal, value);
                if (bestVal > value) bestVal = value;
            }
        }
        return bestVal;
    }
}

int getMinimaxAction(std::vector<double>& board) {
    int bestVal = 1000;
    int bestMove = -1;
    for (int i = 0; i < 9; i++) {
        if (board[i] == -1) {
            board[i] = 0;
            int moveVal = minimax(board, true);
            board[i] = -1;
            if (moveVal < bestVal) {
                bestMove = i;
                bestVal = moveVal;
            }
        }
    }
    return bestMove;
}