#include <iostream>
#include <vector>

using namespace std;

class CycleQueue {
public:
    explicit CycleQueue(int n) : n_(n), arr_(n) {}

    bool enqueue(int element) {
        if (isFull()) return false;
        arr_[tail_] = element;
        tail_ = (tail_ + 1) % n_;
        return true;
    }

    int dequeue() {
        if (isEmpty()) return -1;
        int element = arr_[head_];
        head_ = (head_ + 1) % n_;
        return element;
    }

private:
    int head_ = 0, tail_ = 0;
    int n_;
    std::vector<int> arr_;

    bool isFull() const { return (tail_ + 1) % n_ == head_; }
    bool isEmpty() const { return head_ == tail_; }
};

int main() {
    CycleQueue cq(10);
    cout << cq.dequeue() << endl;

    cq.enqueue(2);
    cq.enqueue(3);
    cout << cq.dequeue() << endl;
    cout << cq.dequeue() << endl;
    cout << cq.dequeue() << endl;

    return 0;
}
