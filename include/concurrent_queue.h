#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H

#include <mutex>
#include <queue>

enum class Priority { LOW,
    MEDIUM,
    HIGH };

template <typename T>
class ConcurrentQueue {

public:
    void push(const T& val, const Priority priority = Priority::MEDIUM)
    {
        std::unique_lock lck(mut);

        if (q.size() >= max) {
            check_full.wait(lck, [this]() { return q.size() < max; });
        }

        q.push({ priority, val });
        check_empty.notify_all();
    }

    T front()
    {
        std::unique_lock lck(mut);

        if (q.empty()) {
            check_empty.wait(lck, [this]() { return !q.empty(); });
        }

        auto val = q.top();
        q.pop();
        check_full.notify_all();

        return val.second;
    }

private:
    using PrioPair = std::pair<Priority, T>;

    struct PrioPairCompare {
        bool operator()(const PrioPair& p1, const PrioPair& p2)
        {
            return p1.first < p2.first;
        }
    };

    std::priority_queue<PrioPair, std::vector<PrioPair>, PrioPairCompare> q;
    std::mutex mut;
    size_t max = 20;

    std::condition_variable check_empty;
    std::condition_variable check_full;
};

#endif // CONCURRENTQUEUE_H
