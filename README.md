# OOP-Brain

Bu proje, C++ dilinde nesneye yönelik programlama kullanılarak geliştirilen yapay sinir ağı kütüphanesi projesidir.

## Benim Sorumluluğum

Bu projede benim görevim hata yönetimi ve test birimidir.

## Eklenen Dosyalar

- Exceptions.h
- Tests.h
- Tests.cpp
- main.cpp

## Hata Yönetimi

Projede özel hata sınıfları oluşturulmuştur:

- DimensionMismatchException
- InvalidIndexException

## Test Sistemi

Tests.cpp dosyasında hata yönetimi sınıflarının çalışıp çalışmadığını kontrol eden test fonksiyonları yazılmıştır.

## Çalıştırma

```bash
g++ main.cpp Tests.cpp -o app
./app


## Sınıf Yapısı
- **Matrix** — Matematik motoru, tüm lineer cebir işlemleri
- **BaseLayer** — Soyut temel katman sınıfı
- **DenseLayer** — Tam bağlantılı katman implementasyonu
- **IActivation** — Aktivasyon fonksiyonu arayüzü
- **Sigmoid / ReLU / Tanh / LeakyReLU** — Aktivasyon sınıfları
- **NeuralNetwork** — Katman yönetimi ve tahmin motoru
- **CSVReader** — Veri okuma modülü

## Kullanılan OOP Kavramları
- Kalıtım (Inheritance): BaseLayer → DenseLayer
- Polimorfizm: virtual forward() fonksiyonu
- Soyut Sınıf: IActivation arayüzü
- Kapsülleme: private veri üyeleri
- Bellek Yönetimi: destructor ile heap temizleme

## Grup Üyeleri ve Roller
| Rol | Kişi |
|-----|------|
| Baş Yazılım Mimarı | Semanur Şirin |
| Matematik Motoru Geliştiricisi | Emine Acar |
| Aktivasyon Fonksiyonu Geliştiricisi | Hüseyin Kaan Akkaya |
| Katman Uygulama Uzmanı | Ahmet Gülden |
| Veri ve Dosya İşleme Uzmanı | Gamze Kılıç |
| Hata Yönetimi ve Test Birimi | Sueda Acar |
| Sistem Entegratörü ve Dokümantasyon | |
