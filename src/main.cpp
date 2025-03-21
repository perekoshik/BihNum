#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cmath>
#include "InitBigNumber.hpp"

bool nearlyEqual(double a, double b, double epsilon = 1e-6) {
    return std::fabs(a - b) < epsilon;
}

double toDouble(const LongNum &num) {
    return std::stod(num.toString());
}

bool nearlyEqualLong(const LongNum &a, const LongNum &b, double epsilon = 1e-6) {
    return nearlyEqual(toDouble(a), toDouble(b), epsilon);
}

void runLongTest(const std::string &desc, const LongNum &result, const std::string &expectedStr) {
    double resVal = toDouble(result);
    double expVal = std::stod(expectedStr);
    bool ok = nearlyEqual(resVal, expVal);
    std::cout << std::left << std::setw(40) << desc
              << " | result: " << std::setw(15) << result.toString()
              << " | expected: " << expectedStr
              << " | " << (ok ? "OK" : "FAIL") << std::endl;
}

// ==================== Тесты для LongNum ======================
void testLongNumAddition() {
    std::cout << "=== [LongNum] Addition Tests ===" << std::endl;
    runLongTest("1.0 + 2.0", LongNum("1.0") + LongNum("2.0"), "3.0");
    runLongTest("10.5 + 2.5", LongNum("10.5") + LongNum("2.5"), "13.0");
    runLongTest("-5.0 + 3.0", LongNum("-5.0") + LongNum("3.0"), "-2.0");
    std::cout << std::endl;
}

void testLongNumSubtraction() {
    std::cout << "=== [LongNum] Subtraction Tests ===" << std::endl;
    runLongTest("5.0 - 3.0", LongNum("5.0") - LongNum("3.0"), "2.0");
    runLongTest("2.0 - 5.0", LongNum("2.0") - LongNum("5.0"), "-3.0");
    std::cout << std::endl;
}

void testLongNumMultiplication() {
    std::cout << "=== [LongNum] Multiplication Tests ===" << std::endl;
    runLongTest("3.0 * 4.0", LongNum("3.0") * LongNum("4.0"), "12.0");
    runLongTest("1.5 * 2.0", LongNum("1.5") * LongNum("2.0"), "3.0");
    std::cout << std::endl;
}

void testLongNumDivision() {
    std::cout << "=== [LongNum] Division Tests ===" << std::endl;
    runLongTest("10.0 / 2.0", LongNum("10.0") / LongNum("2.0"), "5.0");
    runLongTest("3.0 / 2.0", LongNum("3.0") / LongNum("2.0"), "1.5");
    try {
        LongNum res = LongNum("5.0") / LongNum("0.0");
        std::cout << "5.0 / 0.0  | " << res.toString() << " | FAIL (exception expected)" << std::endl;
    } catch(const std::exception &ex) {
        std::cout << "5.0 / 0.0  | EXCEPTION: " << ex.what() << " | OK (exception caught)" << std::endl;
    }
    std::cout << std::endl;
}

// ==================== Реализация комплексных чисел на основе LongNum ======================

struct LongComplex {
    LongNum real;
    LongNum imag;
    LongComplex(const std::string &r, const std::string &i) : real(r), imag(i) {}
    LongComplex(const LongNum &r, const LongNum &i) : real(r), imag(i) {}
    LongComplex() : real("0.0"), imag("0.0") {}
};

LongComplex operator+(const LongComplex &a, const LongComplex &b) {
    return LongComplex(a.real + b.real, a.imag + b.imag);
}

LongComplex operator-(const LongComplex &a, const LongComplex &b) {
    return LongComplex(a.real - b.real, a.imag - b.imag);
}

LongComplex operator*(const LongComplex &a, const LongComplex &b) {
    return LongComplex(a.real * b.real - a.imag * b.imag,
                       a.real * b.imag + a.imag * b.real);
}

LongComplex operator/(const LongComplex &a, const LongComplex &b) {
    LongNum denom = b.real * b.real + b.imag * b.imag;
    LongNum realPart = (a.real * b.real + a.imag * b.imag) / denom;
    LongNum imagPart = (a.imag * b.real - a.real * b.imag) / denom;
    return LongComplex(realPart, imagPart);
}

bool nearlyEqualComplex(const LongComplex &a, const LongComplex &b, double eps = 1e-6) {
    return nearlyEqual(toDouble(a.real), toDouble(b.real), eps) &&
           nearlyEqual(toDouble(a.imag), toDouble(b.imag), eps);
}

void runComplexTest(const std::string &desc, const LongComplex &result, const LongComplex &expected) {
    bool ok = nearlyEqualComplex(result, expected);
    std::cout << std::left << std::setw(40) << desc
              << " | result: (" << result.real.toString() << " + " << result.imag.toString() << "i)"
              << " | expected: (" << expected.real.toString() << " + " << expected.imag.toString() << "i)"
              << " | " << (ok ? "OK" : "FAIL") << std::endl;
}

void testLongComplexAddition() {
    std::cout << "=== [LongComplex] Addition Tests ===" << std::endl;
    LongComplex a("1.0", "2.0");
    LongComplex b("3.0", "4.0");
    LongComplex res = a + b;
    runComplexTest(" (1+2i) + (3+4i)", res, LongComplex("4.0", "6.0"));
}

void testLongComplexSubtraction() {
    std::cout << "=== [LongComplex] Subtraction Tests ===" << std::endl;
    LongComplex a("5.0", "7.0");
    LongComplex b("2.0", "3.0");
    LongComplex res = a - b;
    runComplexTest(" (5+7i) - (2+3i)", res, LongComplex("3.0", "4.0"));
}

void testLongComplexMultiplication() {
    std::cout << "=== [LongComplex] Multiplication Tests ===" << std::endl;
    LongComplex a("1.0", "2.0");
    LongComplex b("3.0", "4.0");
    LongComplex res = a * b;
    runComplexTest(" (1+2i) * (3+4i)", res, LongComplex("-5.0", "10.0"));
}

void testLongComplexDivision() {
    std::cout << "=== [LongComplex] Division Tests ===" << std::endl;
    LongComplex a("2.0", "2.0");
    LongComplex b("1.0", "1.0");
    LongComplex res = a / b;
    runComplexTest(" (2+2i) / (1+1i)", res, LongComplex("2.0", "0.0"));
}

int main() {
    std::cout << "********** Testing LongNum and LongComplex **********" << std::endl << std::endl;
    
    // Тесты для LongNum
    testLongNumAddition();
    testLongNumSubtraction();
    testLongNumMultiplication();
    testLongNumDivision();
    
    // Тесты для комплексных чисел
    testLongComplexAddition();
    testLongComplexSubtraction();
    testLongComplexMultiplication();
    testLongComplexDivision();
    
    std::cout << "\n********** Testing Completed **********" << std::endl;
    return 0;
}
