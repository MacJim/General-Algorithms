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

    // Closed interval: [left, right]
    int left = 0;
    int right = nums.size() - 1;    // The initial value of `right` is within range when using a closed interval.

    while (left <= right) {    // Terminates when `left == (right + 1)` (because we're using a closed interval and `left == right` may be the solution).
        const auto mid = left + (right - left) / 2;
        if (nums[mid] == target) {
            return mid;
        } else if (nums[mid] > target) {
            // Minus 1 (when using a closed interval) to prevent testing the same incorrect number.
            right = mid - 1;
        } else {
            // Plus 1 to prevent testing the same incorrect number.
            left = mid + 1;
        }
    }

    // Not found when left == (right + 1).
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

    test(searchForANumber, {0,2,4,6,8,10,12}, 3, -1);

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


#pragma mark - Search for leftmost element
/**
 * Search for the first (leftmost) occurrence of `target`.
 *
 * @param nums
 * @param target
 * @return `target`'s leftmost index in `nums` if it exists in `nums`; -1 otherwise.
 */
int searchForLeftmostElement(const std::vector<int>& nums, const int target) {
    if (nums.empty()) {
        return -1;
    }

    // [left, right)
    int left = 0;
    int right = nums.size();    // The initial value of `right` is out of bound when using an open interval. This is because `left` can be out of bound (equals `nums.size()`) on termination.

    while (left < right) {    // Terminates when `left == right`: they may both be out of bound (equal `nums.size()`).
        int mid = left + (right - left) / 2;

        if (nums[mid] >= target) {
            // If it is larger or equals, `mid` will serve as an upper bound (in order to find the lower bound).
            right = mid;
        } else {
            // Note "plus 1" here: add it so that we can return the correct `left` that's equal to or bigger than `target`.
            left = mid + 1;
        }
    }

    if (left == nums.size()) {
        // Out of bound: `target` is too large.
        return -1;
    }
    if (nums[left] != target) {
        // `target` doesn't exist in `nums`.
        return -1;
    }
    return left;

    // The following code returns the index where `target` should be, even if it doesn't exist.
//    if ((left == 0) && (nums[0] != target)) {
//        return -1;
//    } else {
//        return left;
//    }
}


void testSearchForLeftmostElement() {
    std::cout << "Search for leftmost element\n";

    test(searchForLeftmostElement, {0, 1, 2, 3, 4}, 5, -1);
//    test(searchForLeftmostElement, {0, 1, 2, 3, 4}, 5, 5);

    test(searchForLeftmostElement, {0}, 1, -1);
//    test(searchForLeftmostElement, {0}, 1, 1);

    for (const auto i: {0,2,4,6}) {
        test(searchForLeftmostElement, {0, 0, 2, 2, 4, 4, 6, 6}, i, i);
    }
    for (const auto i: {1,3,5,7}) {
        test(searchForLeftmostElement, {0, 0, 2, 2, 4, 4, 6, 6}, i, -1);
//        test(searchForLeftmostElement, {0, 0, 2, 2, 4, 4, 6, 6}, i, i + 1);
    }
    for (const auto i: {0,2,4,6,8}) {
        test(searchForLeftmostElement, {0, 0, 2, 2, 4, 4, 6, 6, 8, 8}, i, i);
    }
    for (const auto i: {1,3,5,7,9}) {
        test(searchForLeftmostElement, {0, 0, 2, 2, 4, 4, 6, 6, 8, 8}, i, -1);
//        test(searchForLeftmostElement, {0, 0, 2, 2, 4, 4, 6, 6}, i, i + 1);
    }
    for (const auto i: {0,1,2,3,4}) {
        test(searchForLeftmostElement, {0, 1, 2, 3, 4}, i, i);
    }
    for (const auto i: {-3,-2,-1}) {
        test(searchForLeftmostElement, {0, 1, 2, 3, 4}, i, -1);
    }
    for (const auto i: {5,6,7}) {
        test(searchForLeftmostElement, {0, 1, 2, 3, 4}, i, -1);
//        test(searchForLeftmostElement, {0, 1, 2, 3, 4}, i, 5);
    }
    for (const auto i: {0,1,2,3,4,5}) {
        test(searchForLeftmostElement, {0, 1, 2, 3, 4, 5}, i, i);
    }
    for (const auto i: {-3,-2,-1}) {
        test(searchForLeftmostElement, {0, 1, 2, 3, 4, 5}, i, -1);
    }
    for (const auto i: {6,7,8}) {
        test(searchForLeftmostElement, {0, 1, 2, 3, 4, 5}, i, -1);
    }
}


#pragma mark - Search for rightmost element
/**
 * Search for the last (rightmost) occurrence of `target`.
 *
 * @param nums
 * @param target
 * @return `target`'s rightmost index in `nums` if it exists in `nums`; -1 otherwise.
 */
int searchForRightmostElement(const std::vector<int>& nums, const int target) {
    if (nums.empty()) {
        return -1;
    }

    // [left, right)
    int left = 0;
    int right = nums.size();    // The initial value of right is out of bound. This is because `left` can be out of bound at termination.

    while (left < right) {    // Terminates when left == right
        int mid = left + (right - left) / 2;

        if (nums[mid] <= target) {
            // Here we intentionally let `left` get past the upper bound.
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    // Here, `left == right` so we can also return `right`.
    // Minus 1 to get the upper bound back.
    left -= 1;

    if (left == -1) {
        // Out of bound. `target` is too small.
        return -1;
    }
    if (nums[left] != target) {
        // `target` doesn't exist in `nums`.
        return -1;
    }
    return left;

//    if ((left <= 1) && (nums[left] != target)) {
//        return -1;
//    } else {
//        return (left - 1);
//    }
}


void testSearchForRightmostElement() {
    std::cout << "Search for rightmost element\n";

    for (const auto i: {0,2,4,6}) {
        test(searchForRightmostElement, {0, 0, 2, 2, 4, 4, 6, 6}, i, i + 1);
    }
    for (const auto i: {1,3,5,7}) {
        test(searchForRightmostElement, {0, 0, 2, 2, 4, 4, 6, 6}, i, -1);
    }
    for (const auto i: {0,2,4,6,8}) {
        test(searchForRightmostElement, {0, 0, 2, 2, 4, 4, 6, 6, 8, 8}, i, i + 1);
    }
    for (const auto i: {1,3,5,7,9}) {
        test(searchForRightmostElement, {0, 0, 2, 2, 4, 4, 6, 6, 8, 8}, i, -1);
    }
    for (const auto i: {0,1,2,3,4}) {
        test(searchForRightmostElement, {0, 1, 2, 3, 4}, i, i);
    }
    for (const auto i: {-3,-2,-1}) {
        test(searchForRightmostElement, {0, 1, 2, 3, 4}, i, -1);
    }
    for (const auto i: {5,6,7}) {
        test(searchForRightmostElement, {0, 1, 2, 3, 4}, i, -1);
    }
    for (const auto i: {0,1,2,3,4,5}) {
        test(searchForRightmostElement, {0, 1, 2, 3, 4, 5}, i, i);
    }
    for (const auto i: {-3,-2,-1}) {
        test(searchForRightmostElement, {0, 1, 2, 3, 4, 5}, i, -1);
    }
    for (const auto i: {6,7,8}) {
        test(searchForRightmostElement, {0, 1, 2, 3, 4, 5}, i, -1);
    }
}


int main() {
    testSearchForANumber();
    testSearchForLeftmostElement();
    testSearchForRightmostElement();

    return 0;
}
