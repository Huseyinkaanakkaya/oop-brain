#include "Matrix.h"
#include <cmath> // Karekök (sqrt) hesaplamaları için (He ve Xavier algoritmaları)

// ====================================================================================
// 1. İÇ YARDIMCI FONKSİYONLAR (KAPSÜLLEME / ENCAPSULATION)
// ====================================================================================

// Fiziksel bellek tahsisi. 
// Bu fonksiyon, 2 boyutlu matrisin satır ve sütunları için öbek (heap) bellekte yer ayırır.
void Matrix::allocateMemory() {
    m_data = new double*[m_rows];
    for (size_t i = 0; i < m_rows; ++i) {
        m_data[i] = new double[m_cols]{0.0}; // {0.0} ile güvenli ilklendirme
    }
}

// Dinamik bellek temizliği (RAII Prensibi).
// Valgrind testlerinde "Memory Leak" (Bellek Sızıntısı) hatası almamak için kritik fonksiyondur.
// noexcept: Yıkım işlemi sırasında fırlatılabilecek hataların sistemi çökertmesini engeller.
void Matrix::freeMemory() noexcept {
    if (m_data != nullptr) {
        for (size_t i = 0; i < m_rows; ++i) {
            delete[] m_data[i];
        }
        delete[] m_data;
        m_data = nullptr; // Sarkan işaretçi (Dangling pointer) oluşumunu önlemek için sıfırlanır
    }
}

// Güvenlik kilitleri: Matematiksel işlemlerde boyut uyuşmazlığı varsa özel hata fırlatılır 
void Matrix::validateSameDimensions(const Matrix& other) const {
    if (m_rows != other.m_rows || m_cols != other.m_cols) {
        throw DimensionMismatchException("Hata: Matris boyutlari bu islem (toplama/cikarma vb.) icin birebir ayni olmalidir!");
    }
}

void Matrix::validateMultiplication(const Matrix& other) const {
    if (m_cols != other.m_rows) {
        throw DimensionMismatchException("Hata: Nokta carpimi (Dot Product) icin 1. matrisin sutun sayisi 2. matrisin satir sayisina esit olmalidir!");
    }
}

// ====================================================================================
// 2. NESNE YAŞAM DÖNGÜSÜ (RULE OF FIVE - 5'Lİ KURAL ZORUNLULUĞU)
// ====================================================================================

// Sınıfın üyelerini güvenli bir şekilde (hata fırlatmadan) takas eder
void swap(Matrix& first, Matrix& second) noexcept {
    using std::swap;
    swap(first.m_rows, second.m_rows);
    swap(first.m_cols, second.m_cols);
    swap(first.m_data, second.m_data);
}

// Varsayılan ve Parametreli Kurucu (Constructor)
Matrix::Matrix(size_t rows, size_t cols, double initialValue) 
    : m_rows(rows), m_cols(cols), m_data(nullptr) {
    if (m_rows > 0 && m_cols > 0) {
        allocateMemory();
        for (size_t i = 0; i < m_rows; ++i) {
            for (size_t j = 0; j < m_cols; ++j) m_data[i][j] = initialValue;
        }
    }
}

// 1. Derin Kopyalama (Deep Copy Constructor)
// Sığ kopyalamanın (Shallow Copy) yaratacağı bellek çöküşlerini engeller. Yeni bellek alanı açar.
Matrix::Matrix(const Matrix& other) 
    : m_rows(other.m_rows), m_cols(other.m_cols), m_data(nullptr) {
    if (m_rows > 0 && m_cols > 0) {
        allocateMemory();
        for (size_t i = 0; i < m_rows; ++i) {
            for (size_t j = 0; j < m_cols; ++j) m_data[i][j] = other.m_data[i][j];
        }
    }
}

// 2. Atama Operatörü (Copy-and-Swap İdiomu ile Güçlü İstisna Garantisi)
Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) return *this; 
    
    // Önce geçici bir kopya yaratıyoruz (Hata çıkarsa burada çıkar, asıl nesnemiz bozulmaz)
    Matrix tempCopy(other); 
    
    // Kopya başarıyla oluştuysa, kendi içimizdeki verilerle kopyanın verilerini takas ediyoruz
    swap(*this, tempCopy);
    
    // Fonksiyon bittiğinde tempCopy otomatik yıkılacak ve bizim eski çöplerimizi de temizleyecek!
    return *this;
}

// 3. Taşıma Semantiği (Move Constructor)
// Büyük matrislerin gereksiz yere kopyalanmasını önleyerek eğitim (train) hızını artırır.
Matrix::Matrix(Matrix&& other) noexcept 
    : m_rows(other.m_rows), m_cols(other.m_cols), m_data(other.m_data) {
    other.m_rows = 0;
    other.m_cols = 0;
    other.m_data = nullptr; // Sahipliği aldık, eski kaynağı boşalttık
}

// 4. Taşıma Ataması (Move Assignment)
Matrix& Matrix::operator=(Matrix&& other) noexcept {
    if (this == &other) return *this;
    freeMemory(); 
    
    m_rows = other.m_rows;
    m_cols = other.m_cols;
    m_data = other.m_data; 
    
    other.m_rows = 0;
    other.m_cols = 0;
    other.m_data = nullptr;
    
    return *this;
}

// 5. Yıkıcı (Destructor)
Matrix::~Matrix() {
    freeMemory();
}

// ====================================================================================
// 3. YARATIMSAL TASARIM DESENİ: FABRİKA (FACTORY) METODLARI
// ====================================================================================

// Tamamı sıfırlardan oluşan matris üretir (Bias ilklendirmesi için idealdir)
Matrix Matrix::Zeros(size_t rows, size_t cols) {
    return Matrix(rows, cols, 0.0);
}

// Birim matris (Identity Matrix) üretir (Ana köşegen 1, diğerleri 0)
Matrix Matrix::Identity(size_t size) {
    Matrix mat(size, size, 0.0);
    for (size_t i = 0; i < size; ++i) mat(i, i) = 1.0;
    return mat;
}

// Belirtilen aralıkta rastgele sayılarla dolu matris üretir
Matrix Matrix::Random(size_t rows, size_t cols, double min, double max) {
    Matrix mat(rows, cols);
    mat.randomize(min, max);
    return mat;
}

// Veri okuma (I/O) modülünden (örneğin CSV) gelen tek boyutlu std::vector verisini, 
// sinir ağı modülünün işleyebileceği "Sütun Matrisine" dönüştüren köprü metodu.
Matrix Matrix::FromVector(const std::vector<double>& vec) {
    Matrix mat(vec.size(), 1);
    for(size_t i = 0; i < vec.size(); ++i) mat(i, 0) = vec[i];
    return mat;
}

// ====================================================================================
// 4. HÜCRE ERİŞİMİ VE İNDİRGENME (REDUCTION) İŞLEMLERİ
// ====================================================================================

double& Matrix::operator()(size_t row, size_t col) {
    if (row >= m_rows || col >= m_cols) throw std::out_of_range("Matris hücresine erisim siniri asildi!");
    return m_data[row][col];
}

const double& Matrix::operator()(size_t row, size_t col) const {
    if (row >= m_rows || col >= m_cols) throw std::out_of_range("Matris hücresine erisim siniri asildi (Const)!");
    return m_data[row][col];
}

// Kayıp (Loss) fonksiyonu hesaplamaları için tüm matrisin toplamını alır
double Matrix::sum() const {
    double total = 0.0;
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) total += m_data[i][j];
    }
    return total;
}

// Matrisin genel ortalamasını alır (Mean Squared Error vb. için kullanılır)
double Matrix::mean() const {
    if (size() == 0) return 0.0;
    return sum() / size();
}

double Matrix::max() const {
    if (size() == 0) throw std::logic_error("Bos matrisin icinde maksimum deger aranamaz.");
    double max_val = m_data[0][0];
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            if (m_data[i][j] > max_val) max_val = m_data[i][j];
        }
    }
    return max_val;
}

// ====================================================================================
// 5. YAPAY SİNİR AĞI: AĞIRLIK İLKLENDİRME (INITIALIZATION) STRATEJİLERİ
// ====================================================================================

// Belirli bir minimum ve maksimum aralıkta homojen (tekdüze) rastgele değer atar
void Matrix::randomize(double min, double max) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(min, max);
    apply([&](double) { return dist(gen); });
}

// SOLID - Tek Sorumluluk (Single Responsibility) Prensibi:
// Matris sınıfı algoritmaların formüllerini bilmek zorunda bırakılmadı. 
// Sadece dışarıdan gelen ilklendirici stratejisini çağırır.
void Matrix::initialize(const IWeightInitializer& initializer) {
    initializer.initialize(*this);
}

// Xavier (Glorot) İlklendirmesi: Sigmoid veya Tanh aktivasyonlarında kullanılır.
// Formül: Karekök( 6 / (Girdi_Sayisi + Çıktı_Sayisi) )
// Ağın ileri ve geri beslemesinde sinyalin varyansını sabit tutar.
void XavierInitializer::initialize(Matrix& matrix) const {
    std::mt19937 gen(std::random_device{}());
    double limit = std::sqrt(6.0 / (matrix.rows() + matrix.cols()));
    std::uniform_real_distribution<double> dist(-limit, limit);

    for (size_t i = 0; i < matrix.rows(); ++i) {
        for (size_t j = 0; j < matrix.cols(); ++j) {
            matrix(i, j) = dist(gen);
        }
    }
}

// He İlklendirmesi: Özellikle ReLU aktivasyon fonksiyonu için tasarlanmıştır.
// Formül: Standart Sapma = Karekök( 2 / Girdi_Sayisi )
// Ölü nöron (dead neuron) oluşumunu ve öğrenmenin durmasını büyük ölçüde engeller.
void HeInitializer::initialize(Matrix& matrix) const {
    std::mt19937 gen(std::random_device{}());
    double girdi_sayisi = (matrix.cols() > 0) ? static_cast<double>(matrix.cols()) : 1.0;
    double std_sapma = std::sqrt(2.0 / girdi_sayisi);
    std::normal_distribution<double> dist(0.0, std_sapma);

    for (size_t i = 0; i < matrix.rows(); ++i) {
        for (size_t j = 0; j < matrix.cols(); ++j) {
            matrix(i, j) = dist(gen);
        }
    }
}

// Klasik rastgele atama
void RandomInitializer::initialize(Matrix& matrix) const {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(m_min, m_max);

    for (size_t i = 0; i < matrix.rows(); ++i) {
        for (size_t j = 0; j < matrix.cols(); ++j) {
            matrix(i, j) = dist(gen);
        }
    }
}

// ====================================================================================
// 6. MATEMATİKSEL VE LİNEER CEBİR OPERASYONLARI
// ====================================================================================

Matrix Matrix::operator+(const Matrix& other) const {
    validateSameDimensions(other);
    Matrix result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j) result.m_data[i][j] = m_data[i][j] + other.m_data[i][j];
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    validateSameDimensions(other);
    Matrix result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j) result.m_data[i][j] = m_data[i][j] - other.m_data[i][j];
    return result;
}

// Ağırlık matrisi ile girdi matrisini çarpan hayati fonksiyon (İleri Besleme / Forward Pass)
// *** CPU CACHE (ÖNBELLEK) OPTİMİZASYONU UYGULANDI ***
Matrix Matrix::operator*(const Matrix& other) const {
    validateMultiplication(other);
    
    // Varsayılan kurucu initialValue parametresi ile hücreleri başlangıçta 0 doldurur
    Matrix result(m_rows, other.m_cols, 0.0); 

    // Geleneksel i->j->k döngü sırası yerine i->k->j kullanıldı.
    // Neden? C++ matrisleri (2B dizileri) bellekte Satır-Satır (Row-Major) tutar.
    // 'other' matrisinin elemanlarına 'j' üzerinden ardışık erişmek CPU'nun 
    // Önbellek Iskalama (Cache Miss) oranını düşürür ve performansı dramatik olarak artırır.
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t k = 0; k < m_cols; ++k) {
            double a_ik = m_data[i][k]; // Sık okunan değeri yazmaça (register) al
            for (size_t j = 0; j < other.m_cols; ++j) {
                result.m_data[i][j] += a_ik * other.m_data[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j) result.m_data[i][j] = m_data[i][j] * scalar;
    return result;
}

Matrix Matrix::operator/(double scalar) const {
    if (scalar == 0.0) throw std::invalid_argument("Hata: Matris sifira bolunemez!");
    Matrix result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j) result.m_data[i][j] = m_data[i][j] / scalar;
    return result;
}

Matrix Matrix::operator+(double scalar) const {
    Matrix result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j) result.m_data[i][j] = m_data[i][j] + scalar;
    return result;
}

bool Matrix::operator==(const Matrix& other) const {
    if (m_rows != other.m_rows || m_cols != other.m_cols) return false;
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            if (std::abs(m_data[i][j] - other.m_data[i][j]) > 1e-9) return false;
        }
    }
    return true;
}

// Devrik (Transpoze) Matris: Geri yayılım (Backpropagation) aşamasında ağırlık güncellemesi için gereklidir.
Matrix Matrix::transpose() const {
    Matrix result(m_cols, m_rows);
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j) result.m_data[j][i] = m_data[i][j];
    return result;
}

// Hadamard Çarpımı (Eleman Bazlı Çarpım): Türev hesaplamalarında hata payını gradyanlarla çarpmak için kullanılır.
Matrix Matrix::hadamard(const Matrix& other) const {
    validateSameDimensions(other);
    Matrix result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j) result.m_data[i][j] = m_data[i][j] * other.m_data[i][j];
    return result;
}

// ====================================================================================
// 7. STRATEJİ DESENİ: FONKSİYONEL MÜDAHALE (Aktivasyon Fonksiyonları İçin)
// ====================================================================================

// Mevcut matrisi bozmadan, verilen fonksiyonu (Örn: Sigmoid) hücrelere uygular ve yeni matris döner.
Matrix Matrix::map(std::function<double(double)> func) const {
    Matrix result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j) result.m_data[i][j] = func(m_data[i][j]);
    return result;
}

// Verilen fonksiyonu doğrudan matrisin kendi üzerinde çalıştırır (In-place işlem).
void Matrix::apply(std::function<double(double)> func) {
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j) m_data[i][j] = func(m_data[i][j]);
}

// ====================================================================================
// 8. GLOBAL OPERATÖRLER (Sınıf Dışı Yardımcılar)
// ====================================================================================

// Matrisi konsola (veya dosyaya) standart olarak, sütun hizalamasına dikkat ederek yazdırır.
std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    for (size_t i = 0; i < matrix.rows(); ++i) {
        for (size_t j = 0; j < matrix.cols(); ++j) {
            os << std::setw(10) << std::setprecision(4) << matrix(i, j) << " ";
        }
        os << "\n";
    }
    return os;
}

// Skaler değeri başa yazarak çarpmaya izin veren global aşırı yükleme (Örn: 0.5 * Matris)
Matrix operator*(double scalar, const Matrix& matrix) {
    return matrix * scalar; 
}