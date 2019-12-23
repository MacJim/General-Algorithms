#include <iostream>
#include <stack>
#include <memory>


class FakeQueue {
private:
    /// Temporary stack.
    std::stack<int>* stack1;
    /// Stack used to store elements. Oldest element on top, newest element on bottom.
    std::stack<int>* stack2;

public:
    FakeQueue() {
        stack1 = new std::stack<int>();
        stack2 = new std::stack<int>();
    }

    ~FakeQueue() {
        delete stack1;
        delete stack2;
    }

public:
    void enqueue(int element) {
        while (!stack2->empty()) {
            stack1->push(stack2->top());
            stack2->pop();
        }
        stack2->push(element);
        while (!stack1->empty()) {
            stack2->push(stack1->top());
            stack1->pop();
        }
    }

    int dequeue() {
        int returnValue = stack2->top();
        stack2->pop();
        return returnValue;
    }
};

int main() {
    std::unique_ptr<FakeQueue> fakeQueue (new FakeQueue());
    fakeQueue->enqueue(1);
    fakeQueue->enqueue(2);
    fakeQueue->enqueue(3);
    fakeQueue->enqueue(4);
    fakeQueue->enqueue(5);

    std::cout << fakeQueue->dequeue() << std::endl;
    std::cout << fakeQueue->dequeue() << std::endl;
    std::cout << fakeQueue->dequeue() << std::endl;

    fakeQueue->enqueue(6);
    fakeQueue->enqueue(7);
    fakeQueue->enqueue(8);
    
    std::cout << fakeQueue->dequeue() << std::endl;
    std::cout << fakeQueue->dequeue() << std::endl;
    std::cout << fakeQueue->dequeue() << std::endl;

    fakeQueue->enqueue(9);
    fakeQueue->enqueue(10);

    std::cout << fakeQueue->dequeue() << std::endl;
    std::cout << fakeQueue->dequeue() << std::endl;
    std::cout << fakeQueue->dequeue() << std::endl;
    std::cout << fakeQueue->dequeue() << std::endl;

    return 0;
}