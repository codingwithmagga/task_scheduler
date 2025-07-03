#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <future>

#include "concurrent_queue.h"

using Func = std::function<void()>;
class Threadpool {

public:
    Threadpool(const int numCores);

    ~Threadpool()
    {
        for (auto& thread : threads) {
            thread.join();
        }
    }

    template <typename F, typename... Args>
    void submit(F&& func, Args&&... args)
    {
        if (!running)
            return;

        using ResultType = std::invoke_result_t<F, Args...>;

        // Create a packaged_task to capture the function and arguments
        auto task = std::make_shared<std::packaged_task<ResultType()>>(
            [func = std::forward<F>(func),
                args = std::make_tuple(std::forward<Args>(args)...)]() mutable {
                return std::apply(func, std::move(args));
            });

        // Wrap in a void() function for the queue
        cq.push([task]() { (*task)(); });
    }

    void quit()
    {
        if (!running.exchange(false))
            return;

        // Push empty tasks to wake up all threads
        for (size_t i = 0; i < threads.size(); ++i) {
            cq.push([] { });
        }
    }

private:
    void worker()
    {
        while (running) {
            try {
                auto task = cq.front();
                task();
            } catch (...) {
                if (!running)
                    break;
                // Handle other exceptions if needed
            }
        }
    }

    std::vector<std::thread> threads;
    ConcurrentQueue<Func> cq;
    std::atomic<bool> running = true;
};

#endif // THREADPOOL_H
