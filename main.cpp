#include "fraction.h"
#include <iostream>
#include <limits>
#include <stdexcept>

void Require(bool ok) { if (!ok) throw std::logic_error("Self-check failed"); }
Fraction Make(long whole, unsigned short fraction) {
    Fraction value; value.Init(whole, fraction); return value;
}
void CheckLifecycle() {
    const int before = Fraction::Live();
    {
        Fraction zero;
        Fraction value(2, 5000);
        Fraction copy(value);
        Require(Fraction::Live() == before + 3);
        Require(zero.toString() == "0.0000" && copy.equal(value));
        std::cout << "Default constructor: "; zero.Display();
        std::cout << "Parameter constructor: "; value.Display();
        std::cout << "Copy constructor: "; copy.Display();
        std::cout << "Live objects in scope: " << Fraction::Live() - before << '\n';
        copy.Init(-2, 7500);
        Require(value.toString() == "2.5000" && copy.toString() == "-1.2500");
        std::cout << "Copy independence: original = " << value.toString()
                  << "; copy = " << copy.toString() << '\n';
        bool rejected = false;
        try { Fraction invalid(0, 10000); }
        catch (const std::invalid_argument&) { rejected = true; }
        Require(rejected && Fraction::Live() == before + 3);
    }
    Require(Fraction::Live() == before);
    std::cout << "Destructor self-check: PASS; 3 scope objects destroyed\n";
    std::cout << "Failed constructor self-check: PASS\n";
}
void SelfCheck() {
    Fraction a = Make(2, 5000), b = Make(-2, 7500);
    Require(a.toString() == "2.5000" && b.toString() == "-1.2500");
    Require(a.add(b).toString() == "1.2500");
    Require(a.sub(b).toString() == "3.7500");
    Require(a.mul(b).toString() == "-3.1250");
    Require(b.less(a) && a.greater(b) && a.equal(a));
    Require(Make(-1, 7500).toString() == "-0.2500");
    Require(Make(0, 9999).add(Make(0, 1)).toString() == "1.0000");
    Require(Make(1, 0).sub(Make(0, 1)).toString() == "0.9999");
    Require(Make(0, 1).mul(Make(0, 5000)).toString() == "0.0001");
    Require(Make(-1, 9999).mul(Make(0, 5000)).toString() == "-0.0001");
    bool rejected = false;
    try { a.Init(0, 10000); } catch (const std::invalid_argument&) { rejected = true; }
    Require(rejected && a.toString() == "2.5000");
    rejected = false;
    try { Make(std::numeric_limits<long>::max(), 9999).add(Make(0, 1)); }
    catch (const std::overflow_error&) { rejected = true; }
    Require(rejected); rejected = false;
    try { Make(std::numeric_limits<long>::min(), 0).mul(Make(-1, 0)); }
    catch (const std::overflow_error&) { rejected = true; }
    Require(rejected);
    Require(Make(std::numeric_limits<long>::min(), 0).mul(Make(1, 0))
            .equal(Make(std::numeric_limits<long>::min(), 0)));
    std::cout << "Self-checks: PASS\n";
}
int main() {
    try {
        CheckLifecycle();
        SelfCheck();
        Fraction a, b;
        std::cout << "Enter raw whole and fractional parts of a: "; a.Read();
        std::cout << "\nEnter raw whole and fractional parts of b: "; b.Read();
        std::cout << "\na = "; a.Display(); std::cout << "b = "; b.Display();
        std::cout << "a + b = " << a.add(b).toString() << '\n';
        std::cout << "a - b = " << a.sub(b).toString() << '\n';
        std::cout << "a * b = " << a.mul(b).toString() << '\n';
        std::cout << std::boolalpha << "a == b: " << a.equal(b)
                  << "; a < b: " << a.less(b) << "; a > b: " << a.greater(b) << '\n';
        std::cout << "Raw (-1,7500) = " << Make(-1, 7500).toString() << '\n';
        try { a.Init(0, 10000); }
        catch (const std::invalid_argument& e) {
            std::cout << "Expected error: " << e.what() << '\n';
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n'; return 1;
    }
}
