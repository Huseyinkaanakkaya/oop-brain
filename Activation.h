#include <iostream>
#include <cmath> // exp() ve tanh() için gerekli

// =================================================================
// 1. ADIM: Arayüz (Interface) Tasarımı (12. Hafta Sözleşmesi)
// =================================================================
class IActivation {
public:
    // Saf Sanal Fonksiyon - Override zorunludur.
    virtual double forward(double x) const = 0;

    // Sanal Yıkıcı - Bellek sızıntısını önlemek için ŞART!
    virtual ~IActivation() = default;
};


