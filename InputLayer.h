#ifndef INPUTLAYER_H
#define INPUTLAYER_H
#include "BaseLayer.h"

class InputLayer : public BaseLayer {
public:
    Matrix forward(const Matrix& input) override;
};
#endif
