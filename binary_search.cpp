/*
 * Binary searches.
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


void test(const std::function<int(const std::vector<int>&, const int)>& f, const std::vector<int>& nums, const int target, const int expectedResult) {
    auto result = f(nums, target);

    if (result == expectedResult) {
        std::cout << "[Correct] " << nums << ", " << target << ": " << result << std::endl;
    } else {
        std::cout << "[Wrong] " << nums << ", " << target << ": " << result << " (should be " << expectedResult << ")" << std::endl;
    }
}


#pragma mark - Search for a single number
int searchForANumber(const std::vector<int>& nums, const int target) {
    if (nums.empty()) {
        return -1;
    }

    int left = 0;
    int right = nums.size() - 1;

    while (left <= right) {    // Terminates when left == (right + 1)
        const auto mid = left + (right - left) / 2;
        if (nums[mid] == target) {
            return mid;
        } else if (nums[mid] > target) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    // We know that there's no answer when left == (right + 1).
    return -1;
}


void testSearchForANumber() {
    std::cout << "Search for a single number\n";

    test(searchForANumber, {0}, 1, -1);
    test(searchForANumber, {0}, 0, 0);

    test(searchForANumber, {0,1}, -1, -1);
    test(searchForANumber, {0,1}, 0, 0);
    test(searchForANumber, {0,1}, 1, 1);
    test(searchForANumber, {0,1}, 2, -1);

    test(searchForANumber, {0,2,4}, 3, -1);
    test(searchForANumber, {0,2,4}, 4, 2);

    for (const auto i: {0,1,2,3,4}) {
        test(searchForANumber, {0,1,2,3,4}, i, i);
    }
    for (const auto i: {-3,-2,-1,5,6,7}) {
        test(searchForANumber, {0,1,2,3,4}, i, -1);
    }
    for (const auto i: {0,1,2,3,4,5}) {
        test(searchForANumber, {0,1,2,3,4,5}, i, i);
    }
    for (const auto i: {-3,-2,-1,6,7,8}) {
        test(searchForANumber, {0,1,2,3,4,5}, i, -1);
    }
}


int main() {
    testSearchForANumber();

    return 0;
}
