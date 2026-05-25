#ifndef DENSELAYER_H
#define DENSELAYER_H

#include "BaseLayer.h"
#include "Activation.h"

// Tam Bağlantılı (Fully Connected) Katman
class DenseLayer : public BaseLayer {
private:
    Matrix weights;
    Matrix bias;
    const IActivation* activation; // Bağımlılık Enjeksiyonu (Dependency Injection)

public:
    // Ağırlıklar ve sapmalar varsayılan olarak belirli değerlerle ilklendirilir
    DenseLayer(int inputDim, int outputDim, const IActivation* act, double initWeight = 0.5, double initBias = 0.1);
    ~DenseLayer() override;

    // Matematiksel İleri İletim Akışı: A^(l) = sigma(W^(l) * A^(l-1) + b^(l))
    Matrix forward(const Matrix& input) override;
};

#endif