#ifndef LONGNUM_HPP
#define LONGNUM_HPP

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>

class LongNum {
private:
    std::vector<uint32_t> _digits;

    bool sign;  // Знак числа: false = +, true = -
    uint32_t fraction;

    size_t decimalFractionCount;

    LongNum(std::vector<uint32_t> digits, bool sign, uint32_t fraction);

    void clean_zeros();

    static std::vector<uint32_t> multiplyBy10(const std::vector<uint32_t> &vec);

    static std::vector<uint32_t> addVectors(const std::vector<uint32_t> &a,
                                            const std::vector<uint32_t> &b);

    static std::vector<uint32_t> subVectors(const std::vector<uint32_t> &a,
                                            const std::vector<uint32_t> &b);

    static bool lessVectors(const std::vector<uint32_t> &a,
                            const std::vector<uint32_t> &b);
                            
    static std::vector<uint32_t> multiplyVectors(const std::vector<uint32_t> &a,
                                                 const std::vector<uint32_t> &b);
                                                 
    static std::pair<std::vector<uint32_t>, std::vector<uint32_t>> divideVectors(
                const std::vector<uint32_t> &a,
                const std::vector<uint32_t> &b);

    static std::string bigNumberToDecimalString(const std::vector<uint32_t> &digits);

public:
    LongNum();

    explicit LongNum(const std::string &str);

    LongNum(const LongNum &other);

    LongNum &operator=(const LongNum &other);

    // virtual ~LongNum() = default;

    // ----- Методы -----

    std::string toString() const;

    void setPrecision(uint32_t bits) { fraction = bits; } // Установка двоичных разрядов после запятой

    // ----- Арифметические операторы -----
    friend LongNum operator+(const LongNum &lhs, const LongNum &rhs);
    friend LongNum operator-(const LongNum &lhs, const LongNum &rhs);
    friend LongNum operator*(const LongNum &lhs, const LongNum &rhs);
    friend LongNum operator/(const LongNum &lhs, const LongNum &rhs);

    // ----- Операторы сравнения -----
    friend bool operator==(const LongNum &lhs, const LongNum &rhs);
    friend bool operator!=(const LongNum &lhs, const LongNum &rhs);
    friend bool operator<(const LongNum &lhs, const LongNum &rhs);
    friend bool operator>(const LongNum &lhs, const LongNum &rhs);
};

inline LongNum operator""_longnum(long double value) {
    return LongNum(std::to_string(value));
}

#endif