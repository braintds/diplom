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
#include <stdlib.h>
#include <string>

const int inputSize = 9;
const int hiddenSize = 18;
const int outputSize = 9;

class NeuralNetwork {
private:
    std::vector<double> inputLayer;
    std::vector<double> hiddenLayer;
    std::vector<double> outputLayer;
    std::vector<double> hiddenWeights;
    std::vector<double> outputWeights;
    
public:
    NeuralNetwork();   
    double sigmoid(double x);
    double sigmoidDerivative(double x);
    void forwardPropagation();   
    void train(const std::vector<double>& input, const std::vector<double>& targetOutput, double learningRate);   
    std::vector<double> getOutput();
    std::vector<double> predict(const std::vector<double>& input);
    void loadWeights(const std::string& filename);
    void saveWeights(const std::string& filename);
};

#endif