#pragma once
#include <exception>

class DimensionMismatchException : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "Boyutlar uyusmuyor!";
    }
};

class InvalidIndexException : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "Gecersiz matrix indexi!";
    }
};