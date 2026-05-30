#include <iostream>
#include "Exceptions.h"
#include "Tests.h"
#include "NeuralNetwork.h"
#include "DenseLayer.h"
using namespace std;

void testDimensionMismatch() {
    try {
        throw DimensionMismatchException();
    } catch (const DimensionMismatchException& e) {
        cout << "[TEST BASARILI] " << e.what() << endl;
    }
}

void testInvalidIndex() {
    try {
        throw InvalidIndexException();
    } catch (const InvalidIndexException& e) {
        cout << "[TEST BASARILI] " << e.what() << endl;
    }
}

void testNeuralNetwork() {
    NeuralNetwork nn;
    nn.addLayer(new DenseLayer(2, 3));
    nn.addLayer(new DenseLayer(3, 1));
    Matrix input = Matrix::FromVector({1.0, 0.5});
    Matrix output = nn.predict(input);
    cout << "[TEST BASARILI] NeuralNetwork predict calistiridi, "
         << "cikti boyutu: " << output.rows() << "x" << output.cols() << endl;
}

void runAllTests() {
    cout << "Testler baslatiliyor..." << endl;
    testDimensionMismatch();
    testInvalidIndex();
    testNeuralNetwork();
    cout << "Tum testler tamamlandi." << endl;
}
