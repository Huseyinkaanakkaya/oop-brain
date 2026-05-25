#include "DenseLayer.h"

DenseLayer::DenseLayer(int inputDim, int outputDim, const IActivation* act, double initWeight, double initBias)
    : weights(outputDim, inputDim, initWeight), bias(outputDim, 1, initBias), activation(act) {}

DenseLayer::~DenseLayer() {

}

Matrix DenseLayer::forward(const Matrix& input) {
    Matrix z = (weights * input) + bias;
    
    return activation->forward(z);
}