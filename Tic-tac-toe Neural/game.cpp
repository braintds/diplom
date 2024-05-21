#include "game.h"

void printBoard(std::vector<double> board)
{
    for (size_t i = 0; i < board.size(); i++)
    {
        switch ((int)board[i])
        {
        case 0:
            std::cout << "0 ";
            break;
        case 1:
            std::cout << "X ";
            break;
        case -1:
            std::cout << "- ";
            break;
        }
        if ((i + 1) % 3 == 0)
        {
            std::cout << "\n";
        }
    }
}

int makeTurnAI(myneural::NeuralNetwork network, std::vector<double> board)
{
    int turn = getBestMove(board, network);
    if(turn>=0 && turn<=8) return turn;
    std::cerr<<"Invalid AI turn!\n";
    return -1;
}

// bot == true - make a random turn;
int makeTurnBot(bool bot, std::vector<double> board)
{
    if (bot)
    {
        int random = rand() % 10;
        if (board[random] != -1)
        {
            for (size_t i = 0; i < board.size(); i++)
            {
                if (board[i] == -1)
                {
                    return i;
                }
            }
        }
        return random;
    }

    int turn;
    std::cout << "Make your turn!: ";
    std::cin >> turn;
    return turn;
}

// 1-win AI, 0-win player/bot
int checkWin(std::vector<double> board)
{
    int i{0};
    //line check
    do
    {
        //std::cout << i << "\n";
        if ((board[i] == board[i + 1] && board[i] == board[i + 2]) && (board[i] == 1 || board[i] == 0))
        {
            switch ((int)board[i])
            {
            case 0:
                return 0;
            case 1:
                return 1;
            default:
                break;
            }
        }
        i += 3;
    } while (i < board.size());


    if (((board[0] == board[4] && board[0] == board[8]) || (board[2] == board[4] && board[2] == board[6])) && (board[i] == 1 || board[i] == 0))
    {
        switch ((int)board[i])
        {
        case 0:
            return 0;
        case 1:
            return 1;
        default:
            break;
        }
    }
    i=0;
    do
    {
        if ((board[i] == board[i + 3] && board[i] == board[i + 6]) && (board[i] == 1 || board[i] == 0))
        {
            switch ((int)board[i])
            {
            case 0:
                return 0;
            case 1:
                return 1;
            default:
                break;
            }
        }
        i++;
    } while (i<3);
    
    return -1;
}

bool checkDraw(std::vector<double> board)
{

    for (size_t i = 0; i < board.size(); i++)
    {
        if (board[i] == -1)
            return false;
    }
    return true;
}

int game(myneural::NeuralNetwork network)
{
    bool bot =true;
    char ans;
    //std::cout<<"Who will play? AI - y/yourself - n (y/n) ";
    //std::cin>>ans;
   //std::cout<<"\n";
    if(ans=='y') bot=true;

        std::vector<double> board(9, -1);
        int AIturn{0};
        do
        {
            AIturn = makeTurnAI(network, board);
            // std::cout<<AIturn<<"- AI turn\n";
            if (AIturn == -1)
            {
                std::cerr << "Something wrong with AI";
                //return;
            }
            board[AIturn] = 1;
            // printBoard(board);
            //std::cout << "\n";

            board[makeTurnBot(bot, board)] = 0;
            // printBoard(board);

            switch (checkWin(board))
            {
            case 0:
                //std::cout << "YOU WIN!";
                return 0;
            case 1:
                //std::cout << "AI WIN!";
                return 1;
                
            default:
                break;
            }
            if (checkDraw(board))
            {
                //std::cout << "DRAW!\n";
                return -1;
            }
            // std::cout<<"\n\n";
        } while (1);
    


}