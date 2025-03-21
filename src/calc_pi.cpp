#include <iostream>
#include <cstdlib>
#include <cmath>
#include "InitBigNumber.hpp"
 
LongNum arctan(const LongNum &x, int max_iterations) {
    LongNum result = x;
    LongNum term = x;
    LongNum x2 = x * x;
    for (int k = 1; k <= max_iterations; ++k) {
        term = term * x2;
        std::string denom_str = std::to_string(2 * k + 1) + ".0";
        LongNum denom(denom_str);
        LongNum frac = term / denom;
        if (frac.toString() == "0") break;
        if (k % 2 == 0) {
            result = result + frac;
        } else {
            result = result - frac;
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "calc_pi <number_of_decimal_digits>" << std::endl;
        return 1;
    }
    int decimal_digits = std::atoi(argv[1]);
    if (decimal_digits <= 0) {
        std::cerr << "Number of decimal digits must be positive." << std::endl;
        return 1;
    }
    int bits = std::ceil(decimal_digits * std::log2(10));

    LongNum one("1.0");
    one.setPrecision(bits);

    LongNum five("5.0");
    five.setPrecision(bits);
    LongNum x1 = one / five;
    x1.setPrecision(bits);

    LongNum two_hundred_thirty_nine("239.0");
    two_hundred_thirty_nine.setPrecision(bits);
    LongNum x2 = one / two_hundred_thirty_nine;
    x2.setPrecision(bits);

    int max_iterations = bits * 2;
    LongNum arctan1 = arctan(x1, max_iterations);
    LongNum arctan2 = arctan(x2, max_iterations);

    LongNum sixteen("16.0");
    LongNum four("4.0");
    LongNum term1 = sixteen * arctan1;
    LongNum term2 = four * arctan2;
    LongNum pi = term1 - term2;

    std::string pi_str = pi.toString();
    size_t dot_pos = pi_str.find('.');
    if (dot_pos == std::string::npos) {
        pi_str += ".";
        pi_str.append(decimal_digits, '0');
    } else {
        int current_frac_len = static_cast<int>(pi_str.size() - dot_pos - 1);
        if (current_frac_len < decimal_digits) {
            pi_str.append(decimal_digits - current_frac_len, '0');
        } else if (current_frac_len > decimal_digits) {
            pi_str = pi_str.substr(0, dot_pos + 1 + decimal_digits);
        }
    }
    std::cout << "Pi = " << pi_str << std::endl;
    return 0;
}
