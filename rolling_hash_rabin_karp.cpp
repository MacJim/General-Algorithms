/*
 * Rabin–Karp algorithm string-searching algorithm.
 *
 * https://en.wikipedia.org/wiki/Rabin%E2%80%93Karp_algorithm
 *
 * A rolling hash (also known as recursive hashing or rolling checksum) is a hash function where the input is hashed in a window that moves through the input.
 */

#include <iostream>
#include <string>


class Hasher {
private:
    static const int MOD = 101;
    static const int BASE = 256;

private:
    int length;
    int maxLength;

private:
    int currentHash;

public:
    Hasher(int maxLength): length(0), maxLength(maxLength), currentHash(0) {}

public:
    [[nodiscard]] int getHash() const {
        return currentHash;
    }

    void updateCharacter(char nextCharacter) {
        if (length == maxLength) {
            throw std::runtime_error("Enough initial characters. Call `int updateCharacter(char, char)` instead.");
        }

        currentHash *= BASE;
        currentHash %= MOD;

        currentHash += static_cast<int>(nextCharacter);

        length += 1;
    }

    void updateCharacter(char previousCharacter, char nextCharacter) {
        if (length != maxLength) {
            throw std::runtime_error("Not enough initial characters. Call `int updateCharacter(char)` instead.");
        }

        int previousHashDelta = static_cast<int>(previousCharacter) * pow(BASE, this->maxLength - 1);
        previousHashDelta %= MOD;
        currentHash -= previousHashDelta;

        currentHash *= BASE;
        currentHash %= MOD;

        currentHash += static_cast<int>(nextCharacter);
    }
};


int findWithRollingHash(const std::string& haystack, const std::string& needle) {
    auto needleHasher = Hasher(needle.size());
    for (const auto& c: needle) {
        needleHasher.updateCharacter(c);
    }
    const auto needleHash = needleHasher.getHash();

    // First few characters.
    auto haystackHasher = Hasher(needle.size());
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


void test(const std::string& haystack, const std::string& needle, const int expectedResult) {
    auto result = findWithRollingHash(haystack, needle);

    if (result == expectedResult) {
        std::cout << "[Correct] " << haystack << ", " << needle << ": " << result << std::endl;
    } else {
        std::cout << "[Wrong] " << haystack << ", " << needle << ": " << result << " (should be " << expectedResult << ")" << std::endl;
    }
}


int main() {
    test("abracadabra", "abra", 0);
    test("abracadabra", "cad", 4);
    test("abracadabra", "dab", 6);
    test("abracadabra", "cda", -1);

//    auto hasher1 = Hasher(3);
//    hasher1.updateCharacter('c');
//    hasher1.updateCharacter('a');
//    std::cout << hasher1.updateCharacter('d') << std::endl;
//
//    auto hasher2 = Hasher(3);
//    hasher2.updateCharacter('a');
//    hasher2.updateCharacter('b');
//    hasher2.updateCharacter('r');
//    hasher2.updateCharacter('a', 'a');
//    hasher2.updateCharacter('b', 'c');
//    hasher2.updateCharacter('r', 'a');
//    std::cout << hasher2.updateCharacter('a', 'd') << std::endl;

    return 0;
}
