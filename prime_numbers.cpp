/*
 * Calculate prime numbers.
 */

#include <iostream>
#include <vector>


#pragma mark - Helpers
template <typename T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& v) {
    os << "{";

    for (auto it = v.begin(); it < (v.end() - 1); it += 1) {
        os << *it << ",";
    }
    os << v.back();    // Do not print an extra comma after the final element.

    os << "}";
    return os;
}


#pragma mark - Prime numbers
auto primeNumbers = std::vector<int>({2, 3});

void calculatePrimeNumbers(int n) {
    int currentNum = primeNumbers.back() + 1;

    while (primeNumbers.size() < n) {
        int threshold = static_cast<int>(sqrt(currentNum));    // Only calculate up to sqrt(currentNum)

        bool isPrime = true;
        for (const auto& previousNum: primeNumbers) {
            if (previousNum > threshold) {
                break;
            }

            // Try previous prime numbers.
            if (currentNum % previousNum == 0) {
                isPrime = false;
            }
        }

        if (isPrime) {
            primeNumbers.push_back(currentNum);
        }

        currentNum += 1;
    }
}


int main() {
    calculatePrimeNumbers(100000);
    std::cout << primeNumbers << std::endl;

    return 0;
}
