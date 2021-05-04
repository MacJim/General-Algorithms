/*
 * JQ's Apr 27 interview question.
 *
 * You have a promotion card with n point multipliers e.g. 1, 2, 3, 4.
 * You can only redeem the point multiplier from the beginning or end.
 *
 * You have a list of n purchases with values e.g. 5, 10, 8, 9.
 *
 * You must redeem the purchases in sequence.
 * The point you get in each redemption is multiplier * value.
 *
 * Find the maximum points you can redeem.
 */

#include <iostream>
#include <vector>

#include "helpers/Operators.hpp"


#pragma mark - 1. Choose middle segment first
class Solution {
public:
    int maxPoints(std::vector<int>& multipliers, std::vector<int>& prices) {
        if (multipliers.empty()) {
            return 0;
        } else if (multipliers.size() == 1) {
            return multipliers[0] * prices[0];
        }

        // -1 means uninitialized.
        auto dp = std::vector<std::vector<int>>(multipliers.size(), std::vector<int>(multipliers.size(), -1));

        // Length 2 segments.
        for (size_t mulIndex1 = 0; mulIndex1 <= (multipliers.size() - 2); mulIndex1 += 1) {
            const auto mulIndex2 = mulIndex1 + 1;

//            size_t priceIndex1 = prices.size() - 1;
//            size_t priceIndex2 = priceIndex1 - 1;
            const int& price1 = prices.back();
            const int& price2 = prices[prices.size() - 2];

            const int value1 = price1 * multipliers[mulIndex1] + price2 * multipliers[mulIndex2];
            const int value2 = price2 * multipliers[mulIndex1] + price1 * multipliers[mulIndex2];
            dp[mulIndex1][mulIndex2] = std::max(value1, value2);
        }

        // Longer segments.
        for (size_t segmentLength = 3; segmentLength <= multipliers.size(); segmentLength += 1) {
            for (size_t index1 = 0; index1 <= (multipliers.size() - segmentLength); index1 += 1) {
                size_t index2 = index1 + segmentLength - 1;

                const int& currentPrice = prices[prices.size() - segmentLength];

                const int value1 = dp[index1 + 1][index2] + currentPrice * multipliers[index1];
                const int value2 = dp[index1][index2 - 1] + currentPrice * multipliers[index2];

                dp[index1][index2] = std::max(value1, value2);
            }
        }

        return dp.front().back();
    }
};


void test(const std::vector<int>& multipliers, const std::vector<int>& prices, const int expectedResult) {
    static auto solutionInstance = Solution();

    auto multipliersCopy = multipliers;
    auto pricesCopy = prices;
    auto result = solutionInstance.maxPoints(multipliersCopy, pricesCopy);

    if (result == expectedResult) {
        std::cout << "[Correct] " << multipliers << " " << prices << ": " << result << std::endl;
    } else {
        std::cout << "[Wrong] " << multipliers << " " << prices << ": " << result << " (should be " << expectedResult << ")" << std::endl;
    }
}


int main() {
    test({3, 4}, {8, 9}, 60);
    test({1, 2, 3, 4}, {5, 10, 8, 9}, 88);
    test({4, 8, 1, 3}, {3, 8, 13, 9}, 3 * 3 + 8 * 1 + 13 * 8 + 9 * 4);
    test({4, 8, 1, 3, 1}, {1, 3, 8, 13, 9}, 1 * 1 + 3 * 3 + 8 * 1 + 13 * 8 + 9 * 4);

    return 0;
}
