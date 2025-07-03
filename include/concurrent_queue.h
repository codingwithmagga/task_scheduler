#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H

#include <mutex>
#include <queue>

template <typename T>
class ConcurrentQueue {

public:
    void push(const T& val)
    {
        std::unique_lock lck(mut);

        if (q.size() >= max) {
            check_full.wait(lck, [this]() { return q.size() < max; });
        }

        q.push(val);
        check_empty.notify_all();
    }

    T front()
    {
        std::unique_lock lck(mut);

        if (q.empty()) {
            check_empty.wait(lck, [this]() { return !q.empty(); });
        }

        T val = q.front();
        q.pop();
        check_full.notify_all();

        return val;
    }

private:
    std::queue<T> q;
    std::mutex mut;
    size_t max = 20;

    std::condition_variable check_empty;
    std::condition_variable check_full;
};

#endif // CONCURRENTQUEUE_H
