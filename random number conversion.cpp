// Given random numbers between 0 and 3, generate random numbers between 0 and 5.

#define THREADS_COUNT 4

#include <iostream>
#include <thread>
#include <vector>
#include <map>


void convertRandomNumbersTest() {
    std::vector<int> results (9, 0);
    for (int i = 0; i < 10000000; i += 1) {
        int sourceRandomNumber = arc4random() % 4;

        int destinationIndex = arc4random() % 3;

        std::vector<int> destinationNumbersCandidates;

        switch (sourceRandomNumber) {
        case 0:
            destinationNumbersCandidates = {0, 4, 2};
            break;

        case 1:
            destinationNumbersCandidates = {1, 5, 3};
            break;

        case 2:
            destinationNumbersCandidates = {2, 0, 4};
            break;

        case 3:
            destinationNumbersCandidates = {3, 1, 5};
            break;
        
        default:
            destinationNumbersCandidates = {6, 7, 8};    // Failsafe.
            break;
        }

        results.at(destinationNumbersCandidates.at(destinationIndex)) += 1;
    }

    for (const int& aNumber: results) {
        std::cout << aNumber << " ";
    }
    std::cout << std::endl;
}

int main() {
    // std::thread thread1 (convertRandomNumbersTest);
    // std::thread thread2 (convertRandomNumbersTest);
    // std::thread thread3 (convertRandomNumbersTest);
    // std::thread thread4 (convertRandomNumbersTest);
    // thread1.join();
    // thread2.join();
    // thread3.join();
    // thread4.join();

    std::vector<std::thread> allThreads;
    for (int i = 0; i < THREADS_COUNT; i += 1) {
        std::thread currentThread (convertRandomNumbersTest);
        allThreads.push_back(std::move(currentThread));
    }
    for (int i = 0; i < THREADS_COUNT; i += 1) {
        allThreads.at(i).join();
    }

    return 0;
}