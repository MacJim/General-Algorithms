/*
 * Rabin–Karp algorithm string-searching algorithm.
 *
 * https://en.wikipedia.org/wiki/Rabin%E2%80%93Karp_algorithm
 *
 * A rolling hash (also known as recursive hashing or rolling checksum) is a hash function where the input is hashed in a window that moves through the input.
 */

#include <iostream>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>

#include "helpers/terminal_format.h"


#pragma mark - 1. No hash overflow protection
class HasherV1 {
private:
    static const int MOD = 13;
    static const int BASE = 10;

private:
    int length;
    int maxLength;

private:
    int currentHash;

public:
    HasherV1(int maxLength): length(0), maxLength(maxLength), currentHash(0) {}

public:
    [[nodiscard]] int getHash() const {
        return currentHash;
    }

    void updateCharacter(char nextCharacter) {
        if (length == maxLength) {
            throw std::runtime_error("Enough initial characters. Call `int updateCharacter(char, char)` instead.");
        }

        currentHash *= BASE;
        currentHash += static_cast<int>(nextCharacter);
        currentHash %= MOD;

        length += 1;
    }

    void updateCharacter(char previousCharacter, char nextCharacter) {
        if (length != maxLength) {
            throw std::runtime_error("Not enough initial characters. Call `int updateCharacter(char)` instead.");
        }

        int previousHashDelta = static_cast<int>(previousCharacter) * pow(BASE, this->maxLength - 1);
        previousHashDelta %= MOD;
        currentHash -= previousHashDelta;

        if (currentHash < 0) {
            currentHash += MOD;
        }

        currentHash *= BASE;
        currentHash += static_cast<int>(nextCharacter);
        currentHash %= MOD;
    }
};


#pragma mark - 2. With hash overflow protection
class HasherV2 {
private:
    static const int MOD = 13;
    static const int BASE = 10;

private:
    int length;
    int maxLength;

private:
    int currentHash;

public:
    HasherV2(int maxLength): length(0), maxLength(maxLength), currentHash(0) {}

public:
    [[nodiscard]] int getHash() const {
        return currentHash;
    }

    void updateCharacter(char nextCharacter) {
        if (length == maxLength) {
            throw std::runtime_error("Enough initial characters. Call `int updateCharacter(char, char)` instead.");
        }

        currentHash *= BASE;
        currentHash += static_cast<int>(nextCharacter);
        currentHash %= MOD;

        length += 1;
    }

    void updateCharacter(char previousCharacter, char nextCharacter) {
        if (length != maxLength) {
            throw std::runtime_error("Not enough initial characters. Call `int updateCharacter(char)` instead.");
        }

        int previousHashDelta = static_cast<int>(previousCharacter);
        for (int i = 0; i < (maxLength - 1); i += 1) {
            previousHashDelta *= BASE;
            previousHashDelta %= MOD;
        }
        currentHash -= previousHashDelta;

        if (currentHash < 0) {
            currentHash += MOD;
        }

        currentHash *= BASE;
        currentHash += static_cast<int>(nextCharacter);
        currentHash %= MOD;
    }
};


#pragma mark - Find
int findWithRollingHash(const std::string& haystack, const std::string& needle) {
    auto needleHasher = HasherV2(needle.size());
    for (const auto& c: needle) {
        needleHasher.updateCharacter(c);
    }
    const auto needleHash = needleHasher.getHash();

    // First few characters.
    auto haystackHasher = HasherV2(needle.size());
    for (size_t left = 0; left < needle.size(); left += 1) {
        haystackHasher.updateCharacter(haystack[left]);
    }
    if (haystackHasher.getHash() == needleHash) {
        // Compare.
        bool same = true;
        for (size_t i = 0; i < needle.size(); i += 1) {
            if (haystack[i] != needle[i]) {
                same = false;
                break;
            }
        }

        if (same) {
            return 0;
        }
    }

    // Upcoming characters.
    for (size_t right = needle.size(); right < haystack.size(); right += 1) {
        const size_t left = right - needle.size();

        haystackHasher.updateCharacter(haystack[left], haystack[right]);
        if (haystackHasher.getHash() == needleHash) {
            // Compare.
            bool same = true;
            for (size_t i = 0; i < needle.size(); i += 1) {
                if (haystack[i + left + 1] != needle[i]) {
                    same = false;
                    break;
                }
            }

            if (same) {
                return (left + 1);
            }
        }
    }

    return -1;
}


#pragma mark - Tests
void test(const std::string& haystack, const std::string& needle, const int expectedResult) {
    auto result = findWithRollingHash(haystack, needle);

    if (result == expectedResult) {
        std::cout << terminal_format::OK_GREEN << "[Correct] " << terminal_format::ENDC << haystack << ", " << needle << ": " << result << std::endl;
    } else {
        std::cout << terminal_format::FAIL << terminal_format::BOLD << "[Wrong] " << terminal_format::ENDC << haystack << ", " << needle << ": " << result << " (should be " << expectedResult << ")" << std::endl;
    }
}


std::string generateRandomString(const int length) {
    std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    auto generator = std::default_random_engine(seed);
    auto distribution = std::uniform_int_distribution<>(0, chars.size() - 1);

    auto returnValue = std::string(length, 0);
    std::generate_n(returnValue.begin(), length, [&chars, &generator, &distribution]() {
        const auto index = distribution(generator);
        return chars[index];
    });
//    std::cout << returnValue << std::endl;

    return returnValue;
}


void testHashValue(const int needleLength, const int prefixLength) {
    auto needle = generateRandomString(needleLength);
    auto strWithPrefix = generateRandomString(prefixLength) + needle;

    auto needleHasher = HasherV2(needleLength);
    for (const char& c: needle) {
        needleHasher.updateCharacter(c);
    }

    auto strHasher = HasherV2(needleLength);
    for (size_t i = 0; i < needleLength; i += 1) {
        strHasher.updateCharacter(strWithPrefix[i]);
    }
    for (size_t i = needleLength; i < strWithPrefix.size(); i += 1) {
        strHasher.updateCharacter(strWithPrefix[i - needleLength], strWithPrefix[i]);
    }

    if (needleHasher.getHash() == strHasher.getHash()) {
        std::cout << terminal_format::OK_GREEN << "[Correct] " << terminal_format::ENDC << needle << ", " << strWithPrefix << ": " << needleHasher.getHash() << std::endl;
    } else {
        std::cout << terminal_format::FAIL << terminal_format::BOLD << "[Wrong] " << terminal_format::ENDC << needle << ", " << strWithPrefix << ": " << strHasher.getHash() << " (should be " << needleHasher.getHash() << ")" << std::endl;
    }
}


#pragma mark - Main
int main() {
    // Overflow test cases.
    test("aaaaaacccccuuuuu", "aaaaaccccc", 1);
    test("AAAAAACCCCCUUUUU", "AAAAACCCCC", 1);
    test("AAAAAAAAAACCCCCUUUUU", "AAAAACCCCC", 5);

    test("aacc", "acc", 1);
    test("cbR536vk8", "cbR", 0);
    test("536vk8cbR", "cbR", 6);
    test("abracadabra", "abra", 0);
    test("abracadabra", "cad", 4);
    test("abracadabra", "dab", 6);
    test("abracadabra", "cda", -1);

    for (int i = 0; i < 40; i += 1) {
        testHashValue(3, 1);
    }

//    generateRandomString(6);

//    auto hasher1 = HasherV1(3);
//    hasher1.updateCharacter('c');
//    hasher1.updateCharacter('a');
//    hasher1.updateCharacter('d');
//    std::cout << hasher1.getHash() << std::endl;
//
//    auto hasher2 = HasherV1(3);
//    hasher2.updateCharacter('a');
//    hasher2.updateCharacter('b');
//    hasher2.updateCharacter('r');
//    hasher2.updateCharacter('a', 'a');
//    hasher2.updateCharacter('b', 'c');
//    hasher2.updateCharacter('r', 'a');
//    hasher2.updateCharacter('a', 'd');
//    std::cout << hasher2.getHash() << std::endl;

    return 0;
}
