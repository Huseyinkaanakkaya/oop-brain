#ifndef DENSELAYER_H
#define DENSELAYER_H

#include "BaseLayer.h"
#include "Activation.h"


class DenseLayer : public BaseLayer {
private:
    Matrix weights;
    Matrix bias;
    const IActivation* activation;

public:
    DenseLayer(int inputDim, int outputDim, const IActivation* act, double initWeight = 0.5, double initBias = 0.1);
    ~DenseLayer() override;

    Matrix forward(const Matrix& input) override;
};

#endif