#include <iostream>
#include <vector>
#include <algorithm>


int knapsack(const int totalCapacity, std::vector<int>& weights, std::vector<int>& values) {
    if (weights.size() == 0) {
        // No items.
        return 0;
    }

    auto capacityValues = std::vector<int>(totalCapacity + 1, 0);

    for (size_t i = 0; i < weights.size(); i += 1) {
        const auto weight = weights[i];
        const auto value = values[i];

        for (int capacity = (capacityValues.size() - 1); capacity >= 0; capacity -= 1) {
            if (weight > capacity) {
                // Cannot fit current item into the knapsack.
                break;
            }

            // We're only using a single `capacityValues` array.
            // Thus, Iterate from end to start in `capacityValues`.

            // Choose current item.
            const int remainingCapacity = capacity - weight;
            const int remainingValue = capacityValues[remainingCapacity];
            const int newValue = remainingValue + value;

            capacityValues[capacity] = std::max(newValue, capacityValues[capacity]);
        }
    }

    return capacityValues.back();
}


void test(const int capacity, const std::vector<int>& weights, const std::vector<int>& values, const int expectedResult) {
    auto weightsCopy = weights;
    auto valuesCopy = values;

    auto result = knapsack(capacity, weightsCopy, valuesCopy);
    if (result == expectedResult) {
        std::cout << "Correct!" << std::endl;
    } else {
        std::cout << "Incorrect: " << result << " (should be " << expectedResult << ")" << std::endl;
    }
}


int main() {
    test(165, {23,31,29,44,53,38,63,85,89,82}, {92,57,49,68,60,43,67,84,87,72}, 92+57+49+68+43);
    test(26, {12,7,11,8,9}, {24,13,23,15,16}, 13+23+15);
    test(5, {4,5,1}, {1,2,3}, 4);
    test(4, {4,5,1}, {1,2,3}, 3);
    test(3, {4,5,6}, {1,2,3}, 0);

    return 0;
}
