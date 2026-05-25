#include "Activation.h"

// Sigmoid formülü uygulama: f(x) = 1 / (1 + e^-x)
double Sigmoid::forward(double x) const {
    return 1.0 / (1.0 + std::exp(-x));
}

// ReLU formülü uygulama: x > 0 ise x, değilse 0
double ReLU::forward(double x) const {
    return (x > 0.0) ? x : 0.0;
}

// Tanh formülü uygulama: C++'ın hazır tanh fonksiyonunu çağırıyoruz
double Tanh::forward(double x) const {
    return std::tanh(x);
}
