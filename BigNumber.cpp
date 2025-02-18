#include "InitBigNumber.hpp"
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <cctype>

// --------------------------------------------------------------------------

std::vector<uint32_t> LongNum::addVectors(const std::vector<uint32_t> &a,
                                          const std::vector<uint32_t> &b)
{
    std::vector<uint32_t> result;
    const size_t n = std::max(a.size(), b.size());
    result.resize(n, 0);

    uint64_t carry = 0;
    for (size_t i = 0; i < n; i++) {
        uint64_t av = (i < a.size()) ? a[i] : 0;
        uint64_t bv = (i < b.size()) ? b[i] : 0;
        uint64_t sum = av + bv + carry;
        result[i] = (uint32_t)(sum & 0xFFFFFFFF);
        carry = (sum >> 32);
    }
    if (carry) {
        result.push_back((uint32_t)carry);
    }
    return result;
}


std::vector<uint32_t> LongNum::subVectors(const std::vector<uint32_t> &a,
                                          const std::vector<uint32_t> &b)
{
    // Предполагается a >= b
    std::vector<uint32_t> result = a;
    uint64_t carry = 0;
    for (size_t i = 0; i < result.size(); i++) {
        uint64_t av = (uint64_t)result[i];
        uint64_t bv = (i < b.size()) ? b[i] : 0;
        uint64_t diff = av;
        diff = diff - bv - carry;
        result[i] = (uint32_t)(diff & 0xFFFFFFFF);
        carry = (diff >> 63) & 1;
    }

    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }
    return result;
}

// "а меньше чем b ?"

bool LongNum::lessVectors(const std::vector<uint32_t> &a,
                          const std::vector<uint32_t> &b)
{
    if (a.size() != b.size()) {
        return (a.size() < b.size());
    }

    for (size_t i = a.size(); i > 0; i--) {
        if (a[i-1] < b[i-1]) return true;
        if (a[i-1] > b[i-1]) return false;
    }
    return false; // равны
}

std::vector<uint32_t> LongNum::multiplyVectors(const std::vector<uint32_t> &a,
                                               const std::vector<uint32_t> &b)
{
    std::vector<uint32_t> result(a.size() + b.size(), 0);
    for (size_t i = 0; i < a.size(); i++) {
        uint64_t carry = 0;
        for (size_t j = 0; j < b.size() || carry; j++) {
            uint64_t av = (uint64_t)result[i + j] + (uint64_t)a[i] * (j < b.size() ? b[j] : 0) + carry;
            result[i + j] = (uint32_t)(av & 0xFFFFFFFF);
            carry = av >> 32;
        }
    }
    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }
    return result;
}


std::pair<std::vector<uint32_t>, std::vector<uint32_t>> 
LongNum::divideVectors(const std::vector<uint32_t> &a, 
                       const std::vector<uint32_t> &b)
{
    if (b.size() == 1 && b[0] == 0) {
        throw std::runtime_error("Division by zero in LongNum");
    }
    if (lessVectors(a, b)) {
        return { std::vector<uint32_t>{0}, a };
    }
    std::vector<uint32_t> quotient, remainder;
    remainder = a;
    // O(n^2) :
    quotient.clear();
    quotient.push_back(0);
    while (!lessVectors(remainder, b)) {
        remainder = subVectors(remainder, b);
        uint64_t val = (uint64_t)quotient[0] + 1ULL;
        quotient[0] = (uint32_t)(val & 0xFFFFFFFF);
        uint64_t carry = (val >> 32);
        size_t i = 1;
        while (carry && i < quotient.size()) {
            uint64_t sum = (uint64_t)quotient[i] + carry;
            quotient[i] = (uint32_t)(sum & 0xFFFFFFFF);
            carry = (sum >> 32);
            i++;
        }
        if (carry) {
            quotient.push_back((uint32_t)carry);
        }
    }
    while (quotient.size() > 1 && quotient.back() == 0) {
        quotient.pop_back();
    }
    return { quotient, remainder };
}

// Умножаем 32 битное число на 10 (Необходимо при построении из десятичных цифр).
// возвращаем копию

std::vector<uint32_t> multiplyBy10_impl(const std::vector<uint32_t> &vec) {
    std::vector<uint32_t> result = vec;
    uint64_t carry = 0;
    for (size_t i = 0; i < result.size(); i++) {
        uint64_t cur = (uint64_t)result[i] * 10 + carry;
        result[i] = (uint32_t)(cur & 0xFFFFFFFF);
        carry = (cur >> 32);
    }
    if (carry) {
        result.push_back((uint32_t)carry);
    }
    return result;
}

std::vector<uint32_t> LongNum::multiplyBy10(const std::vector<uint32_t> &vec) {
    return multiplyBy10_impl(vec);
}

std::string LongNum::bigNumberToDecimalString(const std::vector<uint32_t> &digits) 
{
    if (digits.size() == 1 && digits[0] == 0) {
        return "0";
    }
    std::vector<uint32_t> temp = digits;

    // Функция divideBy10
    auto divideBy10 = [&](std::vector<uint32_t> &vec) -> uint64_t {
        uint64_t remainder = 0;
        for (size_t i = vec.size(); i > 0; i--) {
            size_t idx = i - 1;
            uint64_t cur = (uint64_t)vec[idx] + (remainder << 32);
            uint64_t res = cur / 10;
            remainder = cur % 10;
            vec[idx] = (uint32_t)res;
        }
        while (vec.size() > 1 && vec.back() == 0) {
            vec.pop_back();
        }
        return remainder;
    };

    std::string result;
    while (!(temp.size() == 1 && temp[0] == 0)) {
        uint64_t r = divideBy10(temp);
        result.push_back((char)('0' + r));
    }
    std::reverse(result.begin(), result.end());
    return result;
}

// --------------------------------------------------------------------------

LongNum::LongNum(std::vector<uint32_t> digits, bool s, uint32_t frac)
    : _digits(std::move(digits)), sign(s), fraction(frac), decimalFractionCount(0)
{
    clean_zeros();
}

void LongNum::clean_zeros() {
    while (_digits.size() > 1 && _digits.back() == 0) {
        _digits.pop_back();
    }
    if (_digits.empty()) {
        _digits.push_back(0);
        sign = false;
    }
}

// --------------------------------------------------------------------------
LongNum::LongNum()
    : sign(false), fraction(64), decimalFractionCount(0)
{
    _digits.clear();
    _digits.push_back(0);
}

// --------------------------------------------------------------------------
LongNum::LongNum(const std::string &str)
    : sign(false), fraction(64), decimalFractionCount(0)
{
    _digits.clear();
    _digits.push_back(0);

    if (str.empty()) {
        return;
    }

    size_t pos = 0;
    if (str[pos] == '+') {
        pos++;
    } else if (str[pos] == '-') {
        sign = true;
        pos++;
    }

    size_t dotPos = str.find('.');
    std::string intPart, fracPart;
    if (dotPos == std::string::npos) {
        intPart = str.substr(pos);
        fracPart = "";
    } else {
        intPart = str.substr(pos, dotPos - pos);
        fracPart = str.substr(dotPos + 1);
    }
    // Удалим ведущие нули в intPart
    while (intPart.size() > 1 && intPart[0] == '0') {
        intPart.erase(intPart.begin());
    }
    if (intPart.empty()) {
        intPart = "0";
    }

    decimalFractionCount = fracPart.size();

    std::vector<uint32_t> bigInt;
    bigInt.push_back(0);

    for (char c : intPart) {
        if (!std::isdigit(c)) {
            throw std::runtime_error("Ошибка. Проверьте написание символа");
        }
        bigInt = multiplyBy10_impl(bigInt);
        uint64_t tmp = (uint64_t)bigInt[0] + (c - '0');
        bigInt[0] = (uint32_t)(tmp & 0xFFFFFFFF);
        uint64_t carry = tmp >> 32;
        size_t idx = 1;
        while (carry && idx < bigInt.size()) {
            uint64_t sum = (uint64_t)bigInt[idx] + carry;
            bigInt[idx] = (uint32_t)(sum & 0xFFFFFFFF);
            carry = sum >> 32;
            idx++;
        }
        if (carry) {
            bigInt.push_back((uint32_t)carry);
        }
    }

    std::vector<uint32_t> bigFrac;
    bigFrac.push_back(0);
    for (char c : fracPart) {
        if (!std::isdigit(c)) {
            throw std::runtime_error("Ошибка. Проверьте написание символа");
        }
        bigFrac = multiplyBy10_impl(bigFrac);
        uint64_t tmp = (uint64_t)bigFrac[0] + (c - '0');
        bigFrac[0] = (uint32_t)(tmp & 0xFFFFFFFF);
        uint64_t carry = tmp >> 32;
        size_t idx = 1;
        while (carry && idx < bigFrac.size()) {
            uint64_t sum = (uint64_t)bigFrac[idx] + carry;
            bigFrac[idx] = (uint32_t)(sum & 0xFFFFFFFF);
            carry = sum >> 32;
            idx++;
        }
        if (carry) {
            bigFrac.push_back((uint32_t)carry);
        }
    }

    // Объединяем
    std::vector<uint32_t> shiftedInt = bigInt;
    for (size_t i = 0; i < decimalFractionCount; i++) {
        shiftedInt = multiplyBy10_impl(shiftedInt);
    }

    std::vector<uint32_t> bigResult = addVectors(shiftedInt, bigFrac);
    _digits = bigResult;
    clean_zeros();
}

// Конструктор копирования
LongNum::LongNum(const LongNum &other)
{
    _digits = other._digits;
    sign = other.sign;
    fraction = other.fraction;
    decimalFractionCount = other.decimalFractionCount;
}

// Оператор присваивания
LongNum &LongNum::operator=(const LongNum &other)
{
    if (this == &other) {
        return *this;
    }
    _digits = other._digits;
    sign = other.sign;
    fraction = other.fraction;
    decimalFractionCount = other.decimalFractionCount;
    return *this;
}


std::string LongNum::toString() const
{
    bool isZero = (_digits.size() == 1 && _digits[0] == 0);

    std::string decStr = bigNumberToDecimalString(_digits);

    // Вставим знак
    if (sign && !isZero) {
        decStr.insert(decStr.begin(), '-');
    }

    if (decimalFractionCount == 0) {
        return decStr;
    }

    bool negativeSign = (sign && !isZero);
    size_t UnsignedLen = negativeSign ? (decStr.size() - 1) : decStr.size();

    if (UnsignedLen <= decimalFractionCount) {
        std::string s;
        if (negativeSign) {
            s.push_back('-'); 
        }
        s += "0.";
        size_t diff = decimalFractionCount - UnsignedLen;
        for (size_t i = 0; i < diff; i++) {
            s.push_back('0');
        }
        s += negativeSign ? decStr.substr(1) : decStr;
        return s;
    } else {
        size_t splitPos;
        if (negativeSign) {
            splitPos = 1 + (UnsignedLen - decimalFractionCount);
        } else {
            splitPos = (UnsignedLen - decimalFractionCount);
        }
        std::string s = decStr;
        s.insert(splitPos, 1, '.');
        return s;
    }
}

// Оператор +

LongNum operator+(const LongNum &lhs, const LongNum &rhs)
{
    LongNum result;
    if (lhs.sign == rhs.sign) {
        result.sign = lhs.sign;
        result._digits = LongNum::addVectors(lhs._digits, rhs._digits);
    } else {

        bool lhsLess = LongNum::lessVectors(lhs._digits, rhs._digits);
        if (lhsLess) {
            result._digits = LongNum::subVectors(rhs._digits, lhs._digits);
            result.sign = rhs.sign;
        } else {
            result._digits = LongNum::subVectors(lhs._digits, rhs._digits);
            result.sign = lhs.sign;
        }
    }
    result.decimalFractionCount = std::max(lhs.decimalFractionCount,
                                           rhs.decimalFractionCount);
    result.fraction = std::max(lhs.fraction, rhs.fraction);
    result.clean_zeros();
    return result;
}

LongNum operator-(const LongNum &lhs, const LongNum &rhs)
{
    LongNum temp = rhs;
    temp.sign = !rhs.sign;
    return lhs + temp;
}

LongNum operator*(const LongNum &lhs, const LongNum &rhs)
{
    LongNum result;
    result.sign = (lhs.sign != rhs.sign);
    result._digits = LongNum::multiplyVectors(lhs._digits, rhs._digits);

    result.decimalFractionCount = lhs.decimalFractionCount + rhs.decimalFractionCount;
    result.fraction = lhs.fraction + rhs.fraction;
    result.clean_zeros();
    return result;
}

LongNum operator/(const LongNum &lhs, const LongNum &rhs)
{
    if (rhs._digits.size() == 1 && rhs._digits[0] == 0) {
        throw std::runtime_error("Деление на ноль");
    }
    LongNum result;
    result.sign = (lhs.sign != rhs.sign);
    auto divRes = LongNum::divideVectors(lhs._digits, rhs._digits);
    std::vector<uint32_t> quotient = divRes.first;
    std::vector<uint32_t> remainder = divRes.second; // Остаток
    result._digits = quotient;
    result.clean_zeros();
    bool remainderIsZero = (remainder.size() == 1 && remainder[0] == 0);
    if (remainderIsZero) {
        result.decimalFractionCount = 0;
        result.fraction = 0;
        return result;
    }
    const size_t MAX_FRACTION_DIGITS = 40;

    std::string fractionDigits;
    fractionDigits.reserve(MAX_FRACTION_DIGITS);

    for (size_t i = 0; i < MAX_FRACTION_DIGITS; i++) {
        {
            remainder = multiplyBy10_impl(remainder);
        }
        auto dres = LongNum::divideVectors(remainder, rhs._digits);
        std::vector<uint32_t> digitVec = dres.first;
        remainder = dres.second;
        uint32_t d = 0; 
        if (!digitVec.empty()) {
            d = digitVec[0];
        }
        // -------------- test --------------
        if (d > 9) {
            d = 9; 
        }

        fractionDigits.push_back(char('0' + (d % 10)));

        bool zeroRem = (remainder.size() == 1 && remainder[0] == 0);
        if (zeroRem) {
            break;
        }
    }
    std::string intPart = LongNum::bigNumberToDecimalString(result._digits);
    std::string fullStr;
    bool isZeroQ = (quotient.size() == 1 && quotient[0] == 0);
    if (result.sign && !isZeroQ) {
        fullStr.push_back('-');
    }
    fullStr += intPart;
    fullStr.push_back('.');
    fullStr += fractionDigits;
    LongNum finalNum(fullStr);

    finalNum.sign = result.sign;
    finalNum.decimalFractionCount = fractionDigits.size();
    finalNum.fraction = std::max(lhs.fraction, rhs.fraction);
    
    return finalNum;
}



bool operator==(const LongNum &lhs, const LongNum &rhs)
{
    if (lhs.sign != rhs.sign) {
        bool lhsZero = (lhs._digits.size() == 1 && lhs._digits[0] == 0);
        bool rhsZero = (rhs._digits.size() == 1 && rhs._digits[0] == 0);
        if (lhsZero && rhsZero) {
            return true;
        }
        return false;
    }
    if (lhs._digits.size() != rhs._digits.size()) {
        return false;
    }
    for (size_t i = 0; i < lhs._digits.size(); i++) {
        if (lhs._digits[i] != rhs._digits[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const LongNum &lhs, const LongNum &rhs)
{
    return !(lhs == rhs);
}

bool operator<(const LongNum &lhs, const LongNum &rhs)
{
    if (lhs.sign != rhs.sign) {
        return lhs.sign > rhs.sign;
    }
    bool isLess = LongNum::lessVectors(lhs._digits, rhs._digits);
    if (!lhs.sign) {
        return isLess;
    } else {
        return !isLess && lhs != rhs;
    }
}

bool operator>(const LongNum &lhs, const LongNum &rhs)
{
    return (!(lhs < rhs) && lhs != rhs);
}
