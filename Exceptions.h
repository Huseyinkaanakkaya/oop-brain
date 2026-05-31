#pragma once
#include <exception>
#include <string>

class DimensionMismatchException : public std::exception {
private:
    std::string message;
public:
    explicit DimensionMismatchException(const std::string& msg = "Boyutlar uyusmuyor!")
        : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
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
