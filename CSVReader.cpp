#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

CSVReader::CSVReader(const std::string& filePath) : fileName(filePath) {}

std::vector<std::vector<double>> CSVReader::readCSV() {
    std::vector<std::vector<double>> data;
    std::ifstream file(fileName); // Dosyayı bilgisayarın hafızasından açar

    // Eğer dosya adı yanlışsa veya bulunamadıysa uyarı verir
    if (!file.is_open()) {
        std::cerr << "Hata: " << fileName << " dosyasi bulunamadi!" << std::endl;
        return data;
    }

    std::string line;
    // Dosyayı satır satır okuyan ana döngü
    while (std::getline(file, line)) {
        std::vector<double> row;
        std::stringstream ss(line);
        std::string value;

        // Satırdaki sayıları virgüllere göre ayıran iç döngü
        while (std::getline(ss, value, ',')) {
            try {
                row.push_back(std::stod(value)); // Metni (string) ondalıklı sayıya (double) çevirir
            } catch (...) {
                // Eğer sayıda hata varsa (boşluk veya yazı gelirse) o hücreyi atla
                continue;
            }
        }

        if (!row.empty()) {
            data.push_back(row); // Satırı ana veritabanımıza ekle
        }
    }

    file.close(); // Dosyayı kapat
    return data;
}