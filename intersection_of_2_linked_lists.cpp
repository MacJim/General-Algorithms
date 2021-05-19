/*
 * Find the intersection of 2 singly linked lists.
 *
 * Problem by Riko: https://blog.cool2645.com/posts/2021-front-end-interview-log/#%E8%85%BE%E8%AE%AF%E4%BA%8C%E9%9D%A2
 */

#include <iostream>

#include "helpers/ListNode.hpp"
#include "helpers/Operators.hpp"


#pragma mark - 1
/*
 * - When `p1` reaches the end, set it to `head2`
 * - When `p2` reaches the end, set it to `head1`
 *
 * This way, the "uncommon" distances that they travel are equal.
 */
class Solution {
public:
    ListNode* findIntersection(ListNode* head1, ListNode* head2) {
        if ((!head1) || (!head2)) {
            return nullptr;
        }

        bool p1Switched = false;
        bool p2Switched = false;

        auto p1 = head1;
        auto p2 = head2;

        while (true) {
            if (p1 == p2) {
                return p1;
            }

            p1 = p1->next;
            p2 = p2->next;

            if (!p1) {
                if (p1Switched) {
                    // `p1` is already in list 2.
                    return nullptr;
                } else {
                    p1 = head2;
                    p1Switched = true;
                }
            }
            if (!p2) {
                if (p2Switched) {
                    // `p2` is already in list 1.
                    return nullptr;
                } else {
                    p2 = head1;
                    p2Switched = true;
                }
            }
        }
    }
};


void test(ListNode* head1, ListNode* head2, const ListNode* expectedResult) {
    static auto solutionInstance = Solution();

    auto listVec1 = ListHelper::serialize(head1);
    auto listVec2 = ListHelper::serialize(head2);

//    auto head1 = ListHelper::deserialize(listVec1);
//    auto head2 = ListHelper::deserialize(listVec2);
    auto result = solutionInstance.findIntersection(head1, head2);
//    auto resultVec = ListHelper::serialize(result);

    if (result == expectedResult) {
        std::cout << "[Correct] " << listVec1 << ", " << listVec2 << ": " << (result? result->val: 0) << std::endl;
    } else {
        std::cout << "[Wrong] " << listVec1 << ", " << listVec2 << ": " << (result? result->val: 0) << " (should be " << (expectedResult? expectedResult->val: 0) << ")" << std::endl;
    }
}


int main() {
    auto head11 = ListHelper::deserialize({1,1,3});
    auto head12 = ListHelper::deserialize({2,2,2,2});
    head12->next->next->next->next = head11->next->next;
    test(head11, head12, head11->next->next);

    auto head21 = nullptr;
    auto head22 = ListHelper::deserialize({2,2,2});
    test(head21, head22, nullptr);

    auto head31 = ListHelper::deserialize({1,1,1});
    auto head32 = nullptr;
    test(head21, head22, nullptr);

    auto head41 = ListHelper::deserialize({1,3});
    auto head42 = ListHelper::deserialize({2,2,2,2,2,2,2,2});
    head42->next->next->next->next->next->next->next = head41->next;
    test(head41, head42, head41->next);

    return 0;
}
