#pragma once
#ifndef NEURALNETWORK_H_
#define NEURALNETWORK_H_
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>

namespace myneural
{
    struct Neuron
    {
        std::vector<double> weights;
        double output;
    };

    struct NeuralLayer
    {
        std::vector<Neuron> neurons;
    };

    class NeuralNetwork
    {
    private:
        NeuralLayer inputLayer;
        NeuralLayer hiddenLayer;
        NeuralLayer outputLayer;
        double learningRate;

    public:
        NeuralNetwork(int inputSize, int hiddenSize, int outputSize, double rate);
        double sigmoid(double x);
        double calculateOutput(const std::vector<double> &inputs, const std::vector<double> &weights);
        std::vector<double> feedForward(const std::vector<double> &inputs);
        std::vector<double> getHiddenLayerOutputs();
        void train(const std::vector<double>& inputs, double target, double lambda);
        void loadDataset(const std::string &filename, std::vector<std::vector<double>> &inputs, std::vector<double> &targets);
        double calculateAccuracy(const std::vector<std::vector<double>> &inputs, const std::vector<double> &targets);
    };

    int getBestMove(const std::vector<double> &boardState, NeuralNetwork &network);
}

#endif