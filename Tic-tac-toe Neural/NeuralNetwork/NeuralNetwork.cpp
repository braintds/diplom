#include "NeuralNetwork.h"
NeuralNetwork::NeuralNetwork()
{
    inputLayer.resize(inputSize, 0);
        hiddenLayer.resize(hiddenSize, 0);
        outputLayer.resize(outputSize, 0);
        hiddenWeights.resize(inputSize * hiddenSize);
        outputWeights.resize(hiddenSize * outputSize);
        srand(time(0));
        for (int i = 0; i < hiddenWeights.size(); ++i) {
            hiddenWeights[i] = (double)rand() / RAND_MAX;
        }
        for (int i = 0; i < outputWeights.size(); ++i) {
            outputWeights[i] = (double)rand() / RAND_MAX;
        }

}

double NeuralNetwork::sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

double NeuralNetwork::sigmoidDerivative(double x)
{
    return x * (1.0 - x);
}

void NeuralNetwork::forwardPropagation()
{
    for (int i = 0; i < hiddenSize; ++i) {
            double sum = 0;
            for (int j = 0; j < inputSize; ++j) {
                sum += inputLayer[j] * hiddenWeights[j * hiddenSize + i];
            }
            hiddenLayer[i] = sigmoid(sum);
        }
        for (int i = 0; i < outputSize; ++i) {
            double sum = 0;
            for (int j = 0; j < hiddenSize; ++j) {
                sum += hiddenLayer[j] * outputWeights[j * outputSize + i];
            }
            outputLayer[i] = sigmoid(sum);
        }

}

void NeuralNetwork::train(const std::vector<double> &input, const std::vector<double> &targetOutput, double learningRate)
{
        inputLayer = input;
        forwardPropagation();

        std::vector<double> outputErrors(outputSize);
        for (int i = 0; i < outputSize; ++i) {
            outputErrors[i] = targetOutput[i] - outputLayer[i];
        }

        for (int i = 0; i < hiddenSize; ++i) {
            for (int j = 0; j < outputSize; ++j) {
                outputWeights[i * outputSize + j] += learningRate * outputErrors[j] * hiddenLayer[i] * sigmoidDerivative(outputLayer[j]);
            }
        }

        std::vector<double> hiddenErrors(hiddenSize);
        for (int i = 0; i < hiddenSize; ++i) {
            double error = 0;
            for (int j = 0; j < outputSize; ++j) {
                error += outputErrors[j] * outputWeights[i * outputSize + j];
            }
            hiddenErrors[i] = error * sigmoidDerivative(hiddenLayer[i]);
        }

        for (int i = 0; i < inputSize; ++i) {
            for (int j = 0; j < hiddenSize; ++j) {
                hiddenWeights[i * hiddenSize + j] += learningRate * hiddenErrors[j] * inputLayer[i];
            }
        }

}

std::vector<double> NeuralNetwork::getOutput()
{
    return outputLayer;
}

std::vector<double> NeuralNetwork::predict(const std::vector<double> &input)
{
    inputLayer = input;
    forwardPropagation();
    return outputLayer;
}

void NeuralNetwork::saveWeights(const std::string &filename)
{
    std::ofstream file(filename);
    if (file.is_open())
    {
        for (const auto &weight : hiddenWeights)
        {
            file << weight << " ";
        }
        file << std::endl;
        for (const auto &weight : outputWeights)
        {
            file << weight << " ";
        }
        file << std::endl;
        file.close();
    }
}

void NeuralNetwork::loadWeights(const std::string &filename)
{
    std::ifstream file(filename);
    if (file.is_open())
    {
        for (auto &weight : hiddenWeights)
        {
            file >> weight;
        }
        for (auto &weight : outputWeights)
        {
            file >> weight;
        }
        file.close();
    }
}