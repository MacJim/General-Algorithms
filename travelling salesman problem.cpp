/*
 * Travelling Salesman Problem
 * https://en.wikipedia.org/wiki/Travelling_salesman_problem
 */

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <algorithm>    // std::find


#pragma mark - Helpers
template<typename T>
void printVector(const std::vector<T>& vectorToPrint) {
    for (const auto& anElement: vectorToPrint) {
        std::cout << anElement << " ";
    }
    std::cout << std::endl;
}

template <typename T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& v) {
    os << "[";
    for (const auto& element: v) {
        os << " " << element;
    }
    os << " ]";
    return os;
}


#pragma mark - 1. Unfinished
//unsigned int salesman1(std::vector<std::vector<unsigned int>> distances) {
//    const auto citiesCount = distances.size();
//    if (citiesCount <= 1) {
//        return 0;
//    }
//
//    unsigned int minimumCost = -1;
//
//    std::vector<std::pair<std::vector<size_t>, unsigned int>> previousSets = {};
//    std::vector<std::pair<std::vector<size_t>, unsigned int>> currentSets = {};
//
//    // Initialize `previousSets`.
//    // Consider only from start (index 0) to index i.
//    for (size_t i = 1; i < citiesCount; i += 1) {
//        std::vector<size_t> currentSet = {i};
//        previousSets.emplace_back(std::move(currentSet), distances.at(0).at(i));
//    }
//
//    for (const auto& aPreviousSet: previousSets) {
//        const auto& previousCities = aPreviousSet.first;
//        const auto& previousDistance = aPreviousSet.second;
//
//        for (size_t i = 1; i < citiesCount; i += 1) {
//            if (std::find(previousCities.begin(), previousCities.end(), i) != previousCities.end()) {
//                continue;
//            }
//        }
//    }
//
//
////    for (size_t i = 1; i < citiesCount; i += 1) {
////        const unsigned int& currentDistance = distances.at(0).at(i);
////        const unsigned int currentCost = currentDistance;    // TODO:
////        minimumCost = std::min(currentCost, minimumCost);
////    }
//
//    return minimumCost;
//}


#pragma mark - 2. Unfinished
//unsigned int getShortestDistance(std::unordered_set<size_t>& existingCities, size_t targetCity, std::vector<std::vector<unsigned int>> distances) {
//
//}

//unsigned int salesman2(std::vector<std::vector<unsigned int>> distances) {
//    const auto citiesCount = distances.size();
//    if (citiesCount <= 1) {
//        return 0;
//    }
//
//    unsigned int minimumCost = -1;
//
//    /*
//     * Record these variables:
//     * - Shortest distance to each variable.
//     * - Existing points in each set.
//     * - Current set size.
//     */
//    std::map<std::unordered_set<size_t>, unsigned int> previousSets = {};
//    std::map<std::unordered_set<size_t>, unsigned int> currentSets = {};
//
//    // Initialize `previousSets`.
//    // Consider only from start (index 0) to index i.
//    for (size_t i = 1; i < citiesCount; i += 1) {
//        std::unordered_set<size_t> currentSet = {i};
//        const auto& currentDistance = distances.at(0).at(i);
//        previousSets.emplace(std::move(currentSet), currentDistance);
//    }
//
//    for (unsigned int previousSetSize = 1; previousSetSize < citiesCount; previousSetSize += 1) {    // Must not be equal to cities count because we are not counting the starting point in.
//        for (const auto& aPreviousSet: previousSets) {
//            const auto& previousCities = aPreviousSet.first;
//            const auto& previousDistance = aPreviousSet.second;
//
//            for (size_t i = 1; i < citiesCount; i += 1) {
//                if (previousCities.find(i) != previousCities.end()) {
//                    continue;
//                }
//
//                auto newCities = previousCities;
//                newCities.insert(i);
//
////                const auto newDistance = previousDistance + distances.at()
//            }
//        }
//    }
//
//    return 0;
//}


#pragma mark - 3
/*
 * For each vertex, record an array of: (previous vertices, distance from all these vertices to the current vertex).
 *
 * Notes:
 * - The current vertex is not stored.
 */
class Salesman {
private:
    size_t citiesCount;
    std::vector<std::vector<unsigned int>> distances;

    std::vector<std::vector<std::pair<std::vector<size_t>, unsigned int>>> previousVertices;
    std::vector<std::vector<std::pair<std::vector<size_t>, unsigned int>>> currentVertices;


public:
    explicit Salesman(const std::vector<std::vector<unsigned int>>& distances) {
        this->distances = distances;
        this->citiesCount = distances.size();

        this->previousVertices = std::vector<std::vector<std::pair<std::vector<size_t>, unsigned int>>>();
        this->currentVertices = std::vector<std::vector<std::pair<std::vector<size_t>, unsigned int>>>();

        initializeFirstVertices();
    }


public:
    std::pair<unsigned int, std::vector<size_t>> calculate() {
        // 1. Iterate to the last.
        for (size_t i = 2; i < citiesCount; i += 1) {
            calculateNextVertices();
        }

        // 2. Add distances up.
        unsigned int minDistance = -1;
        auto optimalPath = std::vector<size_t>();

        for (size_t i = 1; i < citiesCount; i += 1) {
            const auto& distanceBetweenStartAndEnd = distances.at(0).at(i);

            const auto& pathsAndDistances = currentVertices.at(i);
            for (const auto& pathAndDistance: pathsAndDistances) {
                const auto& path = pathAndDistance.first;
                const auto& pathDistance = pathAndDistance.second;

                const auto& totalDistance = distanceBetweenStartAndEnd + pathDistance;
                if (totalDistance < minDistance) {
                    minDistance = totalDistance;
                    optimalPath = path;
                    optimalPath.push_back(i);    // The end point is not included in the path.
                }
            }
        }

        return std::make_pair(minDistance, optimalPath);
    }


private:
    void initializeFirstVertices() {
        // The initial element is kept blank.
        currentVertices.emplace_back();

        // Elements [1, citiesCount).
        std::vector<size_t> initialElement = {0};
        for (size_t i = 1; i < citiesCount; i += 1) {
            // Distance between 0 and `i`.
            auto currentPair = std::make_pair(initialElement, distances.at(0).at(i));
            auto currentVector = std::vector<std::pair<std::vector<size_t>, unsigned int>>({std::move(currentPair)});
            currentVertices.push_back(std::move(currentVector));
        }
    }


private:
    void resetCurrentVertices() {
        this->currentVertices = std::vector<std::vector<std::pair<std::vector<size_t>, unsigned int>>>(citiesCount, std::vector<std::pair<std::vector<size_t>, unsigned int>>());
    }

    void calculateNextVertices() {
        previousVertices = std::move(currentVertices);
        resetCurrentVertices();

        /*
         * For each vertex:
         *   Find neighbor vertices and distances to them.
         *      Calculate new vertices vector and distance.
         *      Insert this pair into the current vertex's array.
         */
        for (size_t currentID = 1; currentID < citiesCount; currentID += 1) {
            // Distance between all neighbors.
            for (size_t neighborID = 1; neighborID < citiesCount; neighborID += 1) {
                if (currentID == neighborID) {
                    continue;
                }

                const auto& distanceBetweenCurrentAndNeighbor = distances.at(currentID).at(neighborID);

                const auto& previousPathsAndDistances = previousVertices.at(neighborID);
                for (const auto& previousPathAndDistance: previousPathsAndDistances) {
                    const auto& previousPath = previousPathAndDistance.first;
                    const auto& previousDistance = previousPathAndDistance.second;

                    if (std::find(previousPath.begin(), previousPath.end(), currentID) == previousPath.end()) {
                        // The current ID is not in this previous path.
                        auto newPath = previousPath;
                        newPath.push_back(neighborID);

                        const auto newDistance = distanceBetweenCurrentAndNeighbor + previousDistance;

                        auto newPair = std::make_pair(std::move(newPath), newDistance);

                        currentVertices.at(currentID).push_back(std::move(newPair));
                    }
                }
            }
        }
    }
};

//unsigned int salesman(std::vector<std::vector<unsigned int>> distances) {
//    const auto citiesCount = distances.size();
//    if (citiesCount <= 1) {
//        return 0;
//    }
//
////    auto previousSets = std::vector<std::map<std::set<size_t>, unsigned int>>();
////    auto currentSets = std::vector<std::map<std::set<size_t>, unsigned int>>();
//
//    auto previousVertices = std::vector<std::vector<std::pair<std::vector<int>, unsigned int>>>(citiesCount, std);
//    auto currentVertices = std::vector<std::vector<std::pair<std::vector<int>, unsigned int>>>(citiesCount, {});
//
//    std::unordered_set<size_t> initialElement = {0};
//    for (size_t i = 1; i < citiesCount; i += 1) {
//        const auto& currentDistance = distances.at(0).at(i);
//    }
//}


#pragma mark - 4. Possible Improvements
/*
 * - We should prune duplicate entries with larger distances at every step. Maybe using a set to store the previous points is the correct way to go?
 */


#pragma mark - Tests
void test(const std::vector<std::vector<unsigned int>>& distances) {
    auto solutionInstance = Salesman(distances);
    auto result = solutionInstance.calculate();
    std::cout << "Shortest distance: " << result.first << ". Path: " << result.second << std::endl;
}

/// Dataset: https://people.sc.fsu.edu/~jburkardt/datasets/tsp/tsp.html
std::vector<std::vector<unsigned int>> get48CitiesTestCase() {
    auto returnValue = std::vector<std::vector<unsigned int>>();

    auto file = std::ifstream("travelling salesman problem data/att48_distances.txt");
    if (file.is_open()) {
        std::string currentLine;
        while (getline(file, currentLine)) {
            auto currentStrings = std::vector<std::string>();

            std::string currentString;
            for (const auto& c: currentLine) {
                if (c != ' ') {
                    currentString += c;
                } else {
                    if (!currentString.empty()) {
                        currentStrings.push_back(std::move(currentString));
                        currentString = std::string();
                    }
                }
            }
            if (!currentString.empty()) {
                // Don't forget the final one.
                currentStrings.push_back(std::move(currentString));
            }

            auto currentDistances = std::vector<unsigned int>();
            for (const auto& s: currentStrings) {
                unsigned int d = std::stoul(s);
                currentDistances.push_back(d);
            }

            returnValue.push_back(std::move(currentDistances));
        }

        file.close();
    }

    return returnValue;
}


int main() {
    std::vector<std::vector<unsigned int>> distances1 = {
        {0, 25, 30, 30},
        {25, 0, 20, 25},
        {30, 20, 0, 25},
        {30, 25, 25, 0},
    };
    test(distances1);

    std::vector<std::vector<unsigned int>> distances2 = {
        {0, 20, 42, 35},
        {20, 0, 30, 34},
        {42, 30, 0, 12},
        {35, 34, 12, 0},
    };
    test(distances2);

    // Sadly this dataset is too big and uses too much memory (even 32GB is not enough 😭).
//    std::vector<std::vector<unsigned int>> distances3 = get48CitiesTestCase();
//    test(distances3);

    return 0;
}
