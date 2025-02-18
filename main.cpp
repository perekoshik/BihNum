#include <iostream>
#include "InitBigNumber.hpp"

int main()
{
    using namespace std;
    LongNum n1("0123.0124");
    cout << "n1 = " << n1.toString() << endl; 
    {
        std::string bigString = "1.0";
        bigString.append(65, '0');
        bigString += "1";
        LongNum n2(bigString);
        cout << "n2 = " << n2.toString() << endl; 
    }
    auto n3 = 3.14159_longnum;
    cout << "n3 = " << n3.toString() << endl; 

    LongNum a("22.0");
    LongNum b("7.0");
    auto sum = a + b;
    auto diff = a - b;
    auto mul = a * b;
    auto div = a / b;

    cout << "sum  (22.0 + 7.0) = " << sum.toString() << endl;
    cout << "diff (22.0 - 7.0) = " << diff.toString() << endl;
    cout << "mul  (22.0 * 7.0) = " << mul.toString() << endl;
    cout << "div (22.0 / 7.0) = " << div.toString() << endl;


    cout << "22.0 == 7.0? " << (a == b) << endl;
    cout << "2.0 != 7.0? " << (a != b) << endl;
    cout << "22.0 <  7.0? " << (a <  b) << endl;
    cout << "22.0 >  7.0? " << (a >  b) << endl;

    LongNum c("10.5");
    c.setPrecision(128); 
    cout << "c = " << c.toString() << " (fraction=" << 128 << ")" << endl;

    return 0;
}
