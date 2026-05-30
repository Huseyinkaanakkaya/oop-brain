#include <iostream>
#include "NeuralNetwork.h"
#include "DenseLayer.h"
#include "Activation.h"
#include "Tests.h"
#include "CSVReader.h"
using namespace std;

int main() {
    // Testleri çalıştır
    runAllTests();
    
    cout << "\n--- XOR Demo ---" << endl;
    
    // Ağı kur
    Sigmoid sigmoid;
    NeuralNetwork nn;
    nn.addLayer(new DenseLayer(2, 3, &sigmoid));
    nn.addLayer(new DenseLayer(3, 1, &sigmoid));
    
    // XOR girdileri
    Matrix input1 = Matrix::FromVector({0.0, 0.0});
    Matrix input2 = Matrix::FromVector({0.0, 1.0});
    Matrix input3 = Matrix::FromVector({1.0, 0.0});
    Matrix input4 = Matrix::FromVector({1.0, 1.0});
    
    // Tahminleri ekrana yaz
    cout << "XOR(0,0) = " << nn.predict(input1)(0,0) << endl;
    cout << "XOR(0,1) = " << nn.predict(input2)(0,0) << endl;
    cout << "XOR(1,0) = " << nn.predict(input3)(0,0) << endl;
    cout << "XOR(1,1) = " << nn.predict(input4)(0,0) << endl;
    
    cout << "\n--- CSV Okuma Demo ---" << endl;
    
    CSVReader reader("data.csv");
    vector<vector<double>> data = reader.readCSV();
    
    if (!data.empty()) {
        cout << "CSV dosyasi okundu: " << data.size() << " satir bulundu." << endl;
        cout << "Ilk satir: ";
        for (double val : data[0]) {
            cout << val << " ";
        }
        cout << endl;
    } else {
        cout << "CSV dosyasi bos veya bulunamadi." << endl;
    }
    
    return 0;
}
