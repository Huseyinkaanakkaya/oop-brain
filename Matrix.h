#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <iomanip>
#include <functional> // Strateji deseni (std::function) entegrasyonu için
#include <stdexcept>  // Standart hata fırlatma sınıfları (runtime_error vb.) için
#include <random>     // Gelişmiş ağırlık ilklendirme (Xavier/He) matematiği için
#include <string>
#include <vector>     // Dış modüllerle (I/O, CSV okuma) iletişim kurabilmek için

// ====================================================================================
// ÖZEL HATA YÖNETİMİ SINIFI 
// ====================================================================================
// Standart hatalar yerine projenin kendi hata mekanizmasını kurguluyoruz.
// Matris boyut uyuşmazlıklarında (örneğin toplama veya çarpma kuralları ihlal edildiğinde)
// programın çökmesi yerine güvenli bir şekilde bu özel hata fırlatılır.
class DimensionMismatchException : public std::logic_error
{
public:
    explicit DimensionMismatchException(const std::string& message)
        : std::logic_error(message) {}
};

class Matrix; // İleri bildirim (Forward declaration) - Strateji sınıfları için gerekli

// ====================================================================================
// AĞIRLIK BAŞLATMA (INITIALIZATION) STRATEJİ ARAYÜZÜ VE SINIFLARI
// ====================================================================================
// SOLID - Açık/Kapalı (Open/Closed) Prensibi
// Yeni bir ilklendirme algoritması eklenmek istenildiğinde Matrix sınıfına 
// dokunmak yerine sadece bu arayüzden türeyen yeni bir sınıf yazılır.

class IWeightInitializer {
public:
    virtual ~IWeightInitializer() = default;
    // Matris nesnesini referans alarak kendi algoritmasına göre içini dolduracak
    virtual void initialize(Matrix& matrix) const = 0; 
};

// Xavier (Glorot) Algoritması Sınıfı: Sigmoid/Tanh aktivasyonlarında sinyal kaybını önler
class XavierInitializer : public IWeightInitializer {
public:
    void initialize(Matrix& matrix) const override;
};

// He Algoritması Sınıfı: ReLU aktivasyonu için tasarlanmıştır, ölü nöron problemini engeller
class HeInitializer : public IWeightInitializer {
public:
    void initialize(Matrix& matrix) const override;
};

// Klasik Rastgele İlklendirme Sınıfı (Homojen atama)
class RandomInitializer : public IWeightInitializer {
private:
    double m_min;
    double m_max;
public:
    RandomInitializer(double min = -1.0, double max = 1.0) : m_min(min), m_max(max) {}
    void initialize(Matrix& matrix) const override;
};

// ====================================================================================
// MATRIX (MATEMATİK MOTORU) ÇEKİRDEK SINIFI
// ====================================================================================
class Matrix
{
private:
    size_t m_rows;
    size_t m_cols;

    // Standart std::vector yerine dinamik 2B Dizi (Öbek/Heap Bellek)
    // Hocanın işaretçi (pointer) ve bellek yönetimi (memory management) hakimiyetini 
    // ölçebilmesi için bilinçli olarak ham işaretçi kullanılmıştır.
    double** m_data;

    // KAPSÜLLEME (Encapsulation) - Dışarıdan erişime kapalı iç yardımcı fonksiyonlar
    void allocateMemory();
    void freeMemory() noexcept; // Yıkıcı çağrılırken exception fırlatılmasını (çökmeyi) engeller

    // İşlem güvenliği kilitleri
    void validateSameDimensions(const Matrix& other) const;
    void validateMultiplication(const Matrix& other) const;

public:
    // ================================================================================
    // 1. NESNE YAŞAM DÖNGÜSÜ (RULE OF FIVE / 5'Lİ KURAL)
    // ================================================================================
    // Bellek sızıntılarını (Memory Leaks) engellemek ve sığ kopyalama (Shallow Copy)
    // hatalarının önüne geçmek için nesnenin tüm yaşam döngüsü manuel yönetilir.
    
    explicit Matrix(size_t rows = 0, size_t cols = 0, double initialValue = 0.0);
    Matrix(const Matrix& other);                    // 1. Derin Kopyalama (Deep Copy Constructor)
    Matrix& operator=(const Matrix& other);         // 2. Atama Operatörü (Copy Assignment)
    Matrix(Matrix&& other) noexcept;                // 3. Taşıma Yapıcısı (Move Constructor) - Performans için!
    Matrix& operator=(Matrix&& other) noexcept;     // 4. Taşıma Ataması (Move Assignment)
    ~Matrix();                                      // 5. Yıkıcı (Destructor - Çöp toplayıcı)
    
    // Güçlü İstisna Garantisi sağlamak amacıyla nesne üyelerini
    // hata fırlatmadan, güvenle takas eden dost (friend) swap fonksiyonu.
    friend void swap(Matrix& first, Matrix& second) noexcept;

    // ================================================================================
    // 2. YARATIMSAL TASARIM DESENİ: FABRİKA METODLARI (Factory Methods)
    // ================================================================================
    // Bir fonksiyon geriye yeni bir değer (özellikle heap'te yer kaplayan bir Matrix nesnesi)
    // dönüyorsa, bu sonucun havada kalıp kaybolmaması gerekir. C++17 ile gelen [[nodiscard]]
    // niteliği, fonksiyonun dönüş değeri kullanılmazsa derleyicinin uyarı vermesini sağlar.
    // Okunabilirliği artırır ve nesne yaratım sürecini soyutlar.
    [[nodiscard]] static Matrix Zeros(size_t rows, size_t cols);
    [[nodiscard]] static Matrix Identity(size_t size);
    [[nodiscard]] static Matrix Random(size_t rows, size_t cols, double min = -1.0, double max = 1.0);
    
    // Veri okuma (I/O) modülünden gelecek verileri kendi sistemimize entegre eden köprü
    [[nodiscard]] static Matrix FromVector(const std::vector<double>& vec);

    // ================================================================================
    // 3. BİLGİ SAĞLAYICILAR (Getters) - Güvenli (Const) Erişim
    // ================================================================================
    // İnline, derleyiciye bu fonksiyonu çağırmak için bellekte zıplama yapmak yerine, 
    // fonksiyonun içindeki kodu doğrudan çağrıldığı yere yapıştırmasını söyler.
    [[nodiscard]] inline size_t rows() const noexcept { return m_rows; }
	[[nodiscard]] inline size_t cols() const noexcept { return m_cols; }
    [[nodiscard]] inline size_t size() const noexcept { return m_rows * m_cols; } // Toplam hücre sayısı
    [[nodiscard]] inline bool isSquare() const noexcept { return m_rows == m_cols; } // Kare matris kontrolü

    // ================================================================================
    // 4. HÜCRE ERİŞİM OPERATÖRLERİ (Operatör Aşırı Yüklemesi)
    // ================================================================================
    // Not: Bu fonksiyonlar IWeightInitializer strateji sınıflarının dışarıdan matris 
    // hücrelerine güvenle değer atayabilmesi için de kritik rol oynar.
	[[nodiscard]] double& operator()(size_t row, size_t col);             // Yazma izni olan erişim
    [[nodiscard]] const double& operator()(size_t row, size_t col) const; // Sadece okuma izni olan (Const) erişim

    // ================================================================================
    // 5. MATEMATİKSEL İŞLEMLER (Polimorfizm ve Ağ İletişimi)
    // ================================================================================
    [[nodiscard]] Matrix operator+(const Matrix& other) const; // Bias (Sapma) ekleme işlemleri için
    [[nodiscard]] Matrix operator-(const Matrix& other) const; // Hata (Loss) hesaplama işlemleri için
    [[nodiscard]] Matrix operator*(const Matrix& other) const; // Nokta Çarpım (İleri Besleme / Forward Pass)

    [[nodiscard]] Matrix operator*(double scalar) const;   // Öğrenme oranı (Learning Rate) çarpımları için     
    [[nodiscard]] Matrix operator/(double scalar) const;
    [[nodiscard]] Matrix operator+(double scalar) const;

    [[nodiscard]] bool operator==(const Matrix& other) const; // Test birimleri (Unit Test) için eşitlik kontrolü

    // ================================================================================
    // 6. LİNEER CEBİR (Geri Yayılım / Backpropagation Araçları)
    // ================================================================================
    [[nodiscard]] Matrix transpose() const;   // Ağırlık güncellemeleri için Devrik Matris               
    [[nodiscard]] Matrix hadamard(const Matrix& other) const; // Türev hesapları için Eleman Bazlı Çarpım
    
    // ================================================================================
    // 7. İNDİRGENME (REDUCTION) İŞLEMLERİ
    // ================================================================================
    [[nodiscard]] double sum() const;  // Ağın toplam hatasını bulmak için
    [[nodiscard]] double mean() const; // Ortalama Kare Hata (MSE) hesaplamaları için
    [[nodiscard]] double max() const;

    // ================================================================================
    // 8. STRATEJİ TASARIM DESENİ (Strategy Pattern)
    // ================================================================================
    // Open/Closed (Açık/Kapalı) Prensibi: Sınıfın kodunu değiştirmeden, dışarıdan 
    // ReLU, Sigmoid veya Tanh gibi aktivasyon fonksiyonlarını matrise enjekte etmeyi sağlar.
    [[nodiscard]] Matrix map(std::function<double(double)> func) const; // Modifiye edilmiş yeni matris döner
    void apply(std::function<double(double)> func);       // Mevcut matrisin içindeki verileri değiştirir

    // ================================================================================
    // 9. YAPAY ZEKA İLKLENDİRME (Initialization)
    // ================================================================================
    // Matrisin sorumluluğunu azaltır, başlatma işini dış sınıflara devreder.
    void initialize(const IWeightInitializer& initializer);
    
    // Eski basit rastgele atama fonksiyonu (geriye dönük uyumluluk için korunabilir)
    void randomize(double min = -1.0, double max = 1.0);

    // ================================================================================
    // 10. AKIŞ ÇIKIŞI (Console / Stream O/I)
    // ================================================================================
    // Matrisi std::cout ile ekrana doğrudan ve biçimli (hizalanmış) olarak yazdırmak için
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
};

// ====================================================================================
// GLOBAL OPERATÖR (Sınıf Bağımsız)
// ====================================================================================
// Değişme özelliğini sağlamak için (Örn: "Matris * 0.5" yerine "0.5 * Matris" yazabilmek için)
[[nodiscard]] Matrix operator*(double scalar, const Matrix& matrix);

#endif // MATRIX_H