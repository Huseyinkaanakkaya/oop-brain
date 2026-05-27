#include <iostream>
#include "Exceptions.h"
#include "Tests.h"

using namespace std;

void testDimensionMismatch()
{
    try
    {
        throw DimensionMismatchException();
    }
    catch (const DimensionMismatchException& e)
    {
        cout << "[TEST BASARILI] " << e.what() << endl;
    }
}

void testInvalidIndex()
{
    try
    {
        throw InvalidIndexException();
    }
    catch (const InvalidIndexException& e)
    {
        cout << "[TEST BASARILI] " << e.what() << endl;
    }
}

void runAllTests()
{
    cout << "Hata yonetimi testleri baslatiliyor..." << endl;

    testDimensionMismatch();
    testInvalidIndex();

    cout << "Tum hata testleri tamamlandi." << endl;
}