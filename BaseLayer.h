#ifndef BASELAYER_H
#define BASELAYER_H

#include "Matrix.h"

// 7-8. Hafta: Soyut Temel Katman Sınıfı
class BaseLayer {
public:
    // Çok kritik: Türetilmiş sınıfların (DenseLayer vb.) bellek sızıntısı yapmadan 
    // silinebilmesi için yıkıcı virtual olmalıdır (Rubric %20 Hafıza Temizliği kuralı).
    virtual ~BaseLayer() {} 
    
    // 10-11. Hafta: Çalışma zamanı polimorfizmi için sanal fonksiyon
    virtual Matrix forward(const Matrix& input) = 0; 
};

#endif