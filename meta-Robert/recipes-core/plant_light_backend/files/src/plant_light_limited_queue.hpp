#include <queue>

template<typename T>
class LimitedQueue {
public:
    LimitedQueue(size_t maxSize) : maxSize_(maxSize) {}

    void push(const T& value) {
        if (q_.size() == maxSize_) {
            q_.pop();              // remove oldest
        }
        q_.push(value);
    }

    void pop()       { q_.pop(); }
    T& front()       { return q_.front(); }
    const T& front() const { return q_.front(); }
    size_t size()    const { return q_.size(); }
    bool empty()     const { return q_.empty(); }
    size_t maxSize() const { return maxSize_; }


private:
    std::queue<T> q_;
    size_t maxSize_;
};
