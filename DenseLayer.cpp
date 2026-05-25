#include "DenseLayer.h"

DenseLayer::DenseLayer(int inputDim, int outputDim, const IActivation* act, double initWeight, double initBias)
    : weights(outputDim, inputDim, initWeight), bias(outputDim, 1, initBias), activation(act) {}

DenseLayer::~DenseLayer() {
    // activation pointer'ının sahipliği NeuralNetwork veya ana programda olduğu için burada delete edilmez.
}

Matrix DenseLayer::forward(const Matrix& input) {
    // Z = W * A_prev + b
    // Not: Giriş vektörünün (input) boyutunun (inputDim x 1) olduğu kabul edilmiştir.
    Matrix z = (weights * input) + bias;
    
    // A = sigma(Z)
    return activation->forward(z);
}