/*
 * Dijkstra's algorithm.
 */

#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <utility>    // std::pair

#include "helpers/Operators.hpp"
#include "helpers/terminal_format.h"


#pragma mark - Helpers


#pragma mark - 1. Min Heap
std::vector<int> dijkstraRedBlackTree(const int nodeCount, const std::vector<std::pair<int, int>>& edges, const std::vector<int>& distances, const int sourceNode) {
    // Create graph.
    auto neighbors = std::vector<std::unordered_map<int, int>>(nodeCount, std::unordered_map<int, int>());
    for (int i = 0; i < edges.size(); i += 1) {
        const auto& edge = edges[i];
        const auto& distance = distances[i];

        neighbors[edge.first][edge.second] = distance;
        neighbors[edge.second][edge.first] = distance;
    }

    auto returnValue = std::vector<int>(nodeCount, INT_MAX);
    returnValue[sourceNode] = 0;

    // Special note that equal values will not be inserted to the set.
    auto cmp = [&returnValue] (const int& lhs, const int& rhs) -> bool {
        if (returnValue[lhs] == returnValue[rhs]) {
            // This is mandatory to deal with the equal values.
            return lhs < rhs;
        } else {
            return returnValue[lhs] < returnValue[rhs];
        }
    };
    /// Unvisited nodes list.
    auto unvisitedNodes = std::set<int, decltype(cmp)>(cmp);
    for (int i = 0; i < nodeCount; i += 1) {
        unvisitedNodes.insert(i);
    }

    while (!unvisitedNodes.empty()) {
        int currentNode = *unvisitedNodes.begin();
        const int& currentDistance = returnValue[currentNode];
        const auto& currentNeighbors = neighbors[currentNode];

        for (const auto& [neighbor, distance]: currentNeighbors) {
            if (unvisitedNodes.count(neighbor)) {
                // Update neighbor if it's unvisited.
                const int newTotalDistance = currentDistance + distance;
                if (newTotalDistance < returnValue[neighbor]) {
                    // Remember to update the order of `unvisitedNodes`.
                    unvisitedNodes.erase(neighbor);
                    returnValue[neighbor] = newTotalDistance;
                    unvisitedNodes.insert(neighbor);
                }
            }
        }

        // Mark current node as visited.
        unvisitedNodes.erase(unvisitedNodes.begin());
    }

    return returnValue;
}


void test(const int nodeCount, const std::vector<std::pair<int, int>>& edges, const std::vector<int>& distances, const int sourceNode, const std::vector<int>& expectedResult) {
    auto result = dijkstraRedBlackTree(nodeCount, edges, distances, sourceNode);
    if (result == expectedResult) {
        std::cout << terminal_format::OK_GREEN << "[Correct]" << terminal_format::ENDC << std::endl;
    } else {
        std::cout << terminal_format::FAIL << "[Wrong] " << terminal_format::ENDC << result << " (should be " << expectedResult << ")" << std::endl;
    }
}


int main() {
    test(5, {{0,1},{0,2},{1,2},{2,3},{1,3},{1,4},{3,4}}, {3,1,7,2,5,1,7}, 2, {1,4,0,2,5});
    test(5, {{0,1},{0,2},{1,2},{2,3},{1,3},{1,4},{3,4}}, {3,1,7,2,5,1,7}, 0, {0,3,1,3,4});
    test(5, {{0,1},{0,2},{1,2},{2,3},{1,3},{1,4},{3,4}}, {3,1,7,2,5,1,7}, 1, {3,0,4,5,1});
    test(5, {{0,1},{0,2},{1,2},{2,3},{1,3},{1,4},{3,4}}, {3,1,7,2,5,1,7}, 3, {3,5,2,0,6});
    test(5, {{0,1},{0,2},{1,2},{2,3},{1,3},{1,4},{3,4}}, {3,1,7,2,5,1,7}, 4, {4,1,5,6,0});

    // Test case from Wikipedia
    test(6, {{0,1},{1,2},{0,2},{0,5},{2,5},{4,5},{3,4},{2,3},{1,3}}, {7,10,9,14,2,9,6,11,15}, 0, {0,7,9,20,20,11});
    test(6, {{0,1},{1,2},{0,2},{0,5},{2,5},{4,5},{3,4},{2,3},{1,3}}, {7,10,9,14,2,9,6,11,15}, 1, {7,0,10,15,21,12});
    test(6, {{0,1},{1,2},{0,2},{0,5},{2,5},{4,5},{3,4},{2,3},{1,3}}, {7,10,9,14,2,9,6,11,15}, 2, {9,10,0,11,11,2});
    test(6, {{0,1},{1,2},{0,2},{0,5},{2,5},{4,5},{3,4},{2,3},{1,3}}, {7,10,9,14,2,9,6,11,15}, 3, {20,15,11,0,6,13});
    test(6, {{0,1},{1,2},{0,2},{0,5},{2,5},{4,5},{3,4},{2,3},{1,3}}, {7,10,9,14,2,9,6,11,15}, 4, {20,21,11,6,0,9});
    test(6, {{0,1},{1,2},{0,2},{0,5},{2,5},{4,5},{3,4},{2,3},{1,3}}, {7,10,9,14,2,9,6,11,15}, 5, {11,12,2,13,9,0});

    return 0;
}
