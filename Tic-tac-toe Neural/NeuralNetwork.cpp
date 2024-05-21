#include "NeuralNetwork.h"

namespace myneural
{

        NeuralNetwork::NeuralNetwork(int inputSize, int hiddenSize, int outputSize, double rate) : learningRate(rate)
        {
            inputLayer.neurons.resize(inputSize);
            hiddenLayer.neurons.resize(hiddenSize);
            outputLayer.neurons.resize(outputSize);

            // Initialize weights randomly
            for (auto &neuron : hiddenLayer.neurons)
            {
                neuron.weights.resize(inputSize);
                for (int i = 0; i < inputSize; ++i)
                {
                    neuron.weights[i] = (rand() % 100) / 100.0;
                }
            }

            for (auto &neuron : outputLayer.neurons)
            {
                neuron.weights.resize(hiddenSize);
                for (int i = 0; i < hiddenSize; ++i)
                {
                    neuron.weights[i] = (rand() % 100) / 100.0;
                }
            }
        }

        double NeuralNetwork::sigmoid(double x)
        {
            return 1 / (1 + exp(-x));
        }

        double NeuralNetwork::calculateOutput(const std::vector<double> &inputs, const std::vector<double> &weights)
        {
            double sum = 0;
            for (int i = 0; i < inputs.size(); ++i)
            {
                sum += inputs[i] * weights[i];
            }
            return sigmoid(sum);
        }

        std::vector<double> NeuralNetwork::feedForward(const std::vector<double> &inputs)
        {
            std::vector<double> outputs;

            if (inputs.size() != inputLayer.neurons.size())
            {
                std::cerr << "Error: Input data size is incorrect\n";
                return outputs;
            }

            for (int i = 0; i < inputLayer.neurons.size(); ++i)
            {
                inputLayer.neurons[i].output = inputs[i];
            }

            for (int i = 0; i < hiddenLayer.neurons.size(); ++i)
            {
                Neuron &neuron = hiddenLayer.neurons[i];
                double sum = 0.0;
                for (int j = 0; j < inputLayer.neurons.size(); ++j)
                {
                    sum += neuron.weights[j] * inputLayer.neurons[j].output;
                }
                neuron.output = sigmoid(sum);
            }

            for (int i = 0; i < outputLayer.neurons.size(); ++i)
            {
                Neuron &neuron = outputLayer.neurons[i];
                double sum = 0.0;
                for (int j = 0; j < hiddenLayer.neurons.size(); ++j)
                {
                    sum += neuron.weights[j] * hiddenLayer.neurons[j].output;
                }
                neuron.output = sigmoid(sum);
                outputs.push_back(neuron.output);
            }

            return outputs;
        }

        std::vector<double> NeuralNetwork::getHiddenLayerOutputs()
        {
            std::vector<double> outputs;
            for (const auto &neuron : hiddenLayer.neurons)
            {
                outputs.push_back(neuron.output);
            }
            return outputs;
        }

        void NeuralNetwork::train(const std::vector<double> &inputs, double target, double lambda)
        {
            // Проверяем корректность размерности входных данных
            if (inputs.size() != inputLayer.neurons.size())
            {
                std::cerr << "Error: Error: Input data size is incorrect!\n";
                return;
            }

            feedForward(inputs);

            // Обновление весов выходного слоя с учетом L2-регуляризации
            for (int i = 0; i < outputLayer.neurons.size(); ++i)
            {
                Neuron &neuron = outputLayer.neurons[i];
                double output = neuron.output;
                double error = (target == i) ? (1 - output) * output * (1 - output) : -output * (1 - output) * output;

                // Добавляем L2-регуляризацию к изменению весов
                for (int j = 0; j < neuron.weights.size(); ++j)
                {
                    double weightUpdate = learningRate * (error * hiddenLayer.neurons[j].output + lambda * neuron.weights[j]);
                    neuron.weights[j] += weightUpdate;
                }
            }

            // Обновление весов скрытого слоя с учетом L2-регуляризации
            for (int i = 0; i < hiddenLayer.neurons.size(); ++i)
            {
                Neuron &neuron = hiddenLayer.neurons[i];
                double output = neuron.output;
                double error = 0;

                for (int j = 0; j < outputLayer.neurons.size(); ++j)
                {
                    error += outputLayer.neurons[j].weights[i] * ((target == j) ? (1 - output) * output * (1 - output) : -output * (1 - output) * output);
                }

                // Добавляем L2-регуляризацию к изменению весов
                for (int j = 0; j < neuron.weights.size(); ++j)
                {
                    double weightUpdate = learningRate * (error * inputs[j] + lambda * neuron.weights[j]);
                    neuron.weights[j] += weightUpdate;
                }
            }
        }

        void NeuralNetwork::loadDataset(const std::string &filename, std::vector<std::vector<double>> &inputs, std::vector<double> &targets)
        {
            std::ifstream file(filename);
            std::string line;
            while (std::getline(file, line))
            {
                std::istringstream iss(line);
                std::vector<double> data;
                double val;
                while (iss >> val)
                {
                    data.push_back(val);
                }
                if (data.size() == 10)
                {
                    std::vector<double> input(data.begin(), data.begin() + 9);
                    inputs.push_back(input);
                    targets.push_back(data[9]);
                }
            }
            file.close();
        }

        double NeuralNetwork::calculateAccuracy(const std::vector<std::vector<double>> &inputs, const std::vector<double> &targets)
        {
            double correct = 0;
            for (int i = 0; i < inputs.size(); ++i)
            {
                std::vector<double> outputs = feedForward(inputs[i]);
                int predictedPosition = std::distance(outputs.begin(), std::max_element(outputs.begin(), outputs.end()));


                if (predictedPosition == targets[i])
                {
                    correct++;
                }
            }
            return correct / inputs.size();
        }

    int getBestMove(const std::vector<double>& boardState, myneural::NeuralNetwork& network) {
    std::vector<double> outputs = network.feedForward(boardState);

    // Ищем наиболее приоритетный ход среди доступных пустых позиций
    int bestMove = -1;
    double bestScore = -1;
    for (int i = 0; i < outputs.size(); ++i) {
        if (boardState[i] == -1 && outputs[i] > bestScore) {
            bestMove = i;
            bestScore = outputs[i];
        }
    }

    // Если не удалось найти доступную пустую позицию, выбираем первую пустую позицию
    if (bestMove == -1) {
        for (int i = 0; i < boardState.size(); ++i) {
            if (boardState[i] == -1) {
                bestMove = i;
                break;
            }
        }
    }

    return bestMove;
}

}