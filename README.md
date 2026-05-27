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