#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <vector>
#include "BaseLayer.h"
#include "Matrix.h"

class NeuralNetwork {
private:
    std::vector<BaseLayer*> layers;

public:
    ~NeuralNetwork();
    void addLayer(BaseLayer* layer);
    Matrix predict(const Matrix& input);
};

#endif
