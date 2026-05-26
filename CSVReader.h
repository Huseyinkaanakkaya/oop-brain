#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <vector>

class CSVReader {
public:
    // Yapıcı fonksiyon: Okunacak dosyanın adını/yolunu alır
    CSVReader(const std::string& filePath);

    // CSV dosyasını gerçekten okuyup satır satır sayıları alan fonksiyon
    std::vector<std::vector<double>> readCSV();

private:
    std::string fileName; // Dosya adını hafızada tutar
};

#endif
