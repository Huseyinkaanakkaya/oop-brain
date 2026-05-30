#include "NeuralNetwork.h"

// Yıkıcı: heap'teki tüm katmanları temizler
NeuralNetwork::~NeuralNetwork() {
    for (BaseLayer* layer : layers) {
        delete layer;
    }
}

// Ağa yeni katman ekler
void NeuralNetwork::addLayer(BaseLayer* layer) {
    layers.push_back(layer);
}

// Girdiyi tüm katmanlardan geçirir, sonucu döner
Matrix NeuralNetwork::predict(const Matrix& input) {
    Matrix output = input;
    for (BaseLayer* layer : layers) {
        output = layer->forward(output);
    }
    return output;
}
