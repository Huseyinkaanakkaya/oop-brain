#ifndef ACTIVATION_H
#define ACTIVATION_H

#include <cmath> // exp() ve tanh() matematiksel fonksiyonları için gerekli

// =================================================================
// 12. Hafta Sözleşmesi: Arayüz (Interface) Tanımı
// =================================================================
class IActivation {
public:
    // Saf sanal fonksiyon. Bu sınıftan türeyen her fonksiyon bunu ezmek zorundadır.
    virtual double forward(double x) const = 0; 

    // Sanal Yıkıcı: Bellek sızıntılarını (memory leak) önlemek için ŞART!
    virtual ~IActivation() = default; 
};

// =================================================================
// Türetilmiş Aktivasyon Fonksiyonları Sınıfları
// =================================================================

// Sigmoid Sınıfı
class Sigmoid : public IActivation {
public:
    double forward(double x) const override;
};

// ReLU Sınıfı
class ReLU : public IActivation {
public:
    double forward(double x) const override;
};

// Tanh Sınıf
class Tanh : public IActivation {
public:
    double forward(double x) const override;
};
class LeakyReLU : public IActivation {
public:
    double forward(double x) const override;
};

#endif // ACTIVATION_H

