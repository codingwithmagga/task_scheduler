#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <future>

#include "concurrent_queue.h"

class Threadpool {

public:
    Threadpool(const int numCores);

    ~Threadpool();

    template <typename F, typename... Args>
    void submitPrio(F&& func, const Priority& priority, Args&&... args)
    {
        if (!running)
            return;

        using ResultType = std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<std::packaged_task<ResultType()>>(
            [func = std::forward<F>(func), ... args = std::forward<Args>(args)]() mutable {
                return std::invoke(func, std::move(args)...);
            });

        cq.push([task]() { (*task)(); }, priority);
    }

    template <typename F, typename... Args>
    void submit(F&& func, Args&&... args)
    {
        submitPrio(std::forward<F>(func), Priority::MEDIUM, std::forward<Args>(args)...);
    }

    void quit();

private:
    void worker();

    std::vector<std::thread> threads;
    ConcurrentQueue<std::function<void()>> cq;
    std::atomic<bool> running = true;
};

#endif // THREADPOOL_H
