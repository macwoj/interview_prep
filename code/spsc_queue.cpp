#include <iostream>
#include <vector>
#include <atomic>

using namespace std;

template<typename T>
class SPSCQueue {
private:
    vector<T> data_;
    alignas(64) atomic<int> head_;
    alignas(64) atomic<int> tail_;

    size_t next(size_t i) {
        return (i+1)%data_.size();
    }
public:

    SPSCQueue(size_t capacity): data_(capacity+1) {
        assert(capacity > 0);
        assert(capacity + 1 > 0);
    }

    bool push(const T& item) {
        auto head = head_.load(memory_order_relaxed);
        auto next_head = next(head);
        //head + 1 == tail FULL
        if (next_head == tail_.load(memory_order_acquire))
            return false;
        data_[head] = item;
        head_.store(memory_order_release);
        return true;
    }

    bool pop(T& value) {
        auto tail = tail_.load(memory_order_relaxed);
        //tail == head EMPTY
        if (tail == head_.load(memory_order_acquire))
            return false;
        value = data_[tail];
        tail_.store(next(tail),memory_order_release);
        return true;
    }
};

int main() {

}
