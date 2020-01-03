/*
 * https://leetcode.com/discuss/interview-question/124724/
 * Expected O(logn) time solution.
 */

#include <iostream>
#include <vector>


// /**
//  * Recursive approach (failed).
//  * 
//  * @param startIndex inclusive.
//  * @param endIndex inclusive.
//  */
// int search1(const std::vector<int>& array, int startIndex, int endIndex, const int& target) {
//     auto elementsCount = endIndex - startIndex + 1;
//     if (elementsCount < 1) {
//         return 0;
//     } else if (elementsCount == 1) {
//         return (array.at(startIndex) == target);
//     }

//     int returnValue = 0;

//     int threshold = elementsCount / 2;

//     int largestElementOnLeft = array.at(threshold - 1);
//     if (largestElementOnLeft >= target) {
//         int leftResult = search1(array, startIndex, threshold - 1, target);
//         returnValue += leftResult;
//     }
    
//     int smallestElementOnRight = array.at(threshold);
//     if (smallestElementOnRight <= target) {
//         int rightResult = search1(array, threshold, endIndex, target);
//         returnValue += rightResult;
//     }

//     return returnValue;
// }


/**
 * The key here is to find a single element that matches `target`.
 */
int search2(const std::vector<int>& array, const int& target) {
    // Special cases.
    auto elementsCount = array.size();
    if (elementsCount < 1) {
        return 0;
    } else if (elementsCount == 1) {
        return (array.at(0) == target);
    }

    if (array.back() < target) {
        return 0;
    }
    if (array.front() > target) {
        return 0;
    }

    int leftIndex = 0;
    int rightIndex = elementsCount - 1;

    int targetIndex = -1;

    while (rightIndex >= leftIndex) {
        if (leftIndex == rightIndex) {
            if (array.at(leftIndex) == target) {
                targetIndex = leftIndex;
            } else {
                break;
            }
        }

        // [leftIndex, threshold), [threshold, rightIndex].
        int threshold = leftIndex + (rightIndex - leftIndex + 1) / 2;
        int thresholdNumber = array.at(threshold);
        if (thresholdNumber == target) {
            targetIndex = threshold;
            break;
        } else if (thresholdNumber > target) {
            // The target should be in the left half.
            rightIndex = threshold - 1;
            continue;
        } else {
            // The target should be in the right half.
            leftIndex = threshold;
            continue;
        }
    }
    
    if (targetIndex == -1) {
        return 0;
    }

    // From the target index, search towards both left and right.
    int returnValue = 1;
    for (int i = targetIndex + 1; i < elementsCount; i += 1) {
        if (array.at(i) == target) {
            returnValue += 1;
        } else {
            break;
        }
    }
    for (int i = targetIndex - 1; i >= 0; i -= 1) {
        if (array.at(i) == target) {
            returnValue += 1;
        } else {
            break;
        }
    }

    return returnValue;
}


int main() {
    auto testArray1 = std::vector<int>({4, 4, 8, 8, 8, 15, 16, 23, 23, 42});
    int targetNumber1 = 8;
    int result1 = search2(testArray1, targetNumber1);
    std::cout << result1 << std::endl;

    auto testArray2 = std::vector<int>({3, 5, 5, 5, 5, 7, 8, 8});
    int targetNumber2 = 6;
    int result2 = search2(testArray2, targetNumber2);
    std::cout << result2 << std::endl;

    auto testArray3 = std::vector<int>({3, 5, 5, 5, 5, 7, 8, 8});
    int targetNumber3 = 8;
    int result3 = search2(testArray3, targetNumber3);
    std::cout << result3 << std::endl;

    auto testArray4 = std::vector<int>({3, 5, 5, 5, 5, 7, 8, 8});
    int targetNumber4 = 7;
    int result4 = search2(testArray4, targetNumber4);
    std::cout << result4 << std::endl;

    return 0;
}
