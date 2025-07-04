#include "thread_pool.h"

Threadpool::Threadpool(const int numCores)
{
    for (int i = 0; i < numCores; ++i) {
        threads.push_back(std::thread(&Threadpool::worker, this));
    }
}

Threadpool::~Threadpool()
{
    for (auto& thread : threads) {
        thread.join();
    }
}

void Threadpool::quit()
{
    if (!running.exchange(false))
        return;

    // Push empty tasks to wake up all threads
    for (size_t i = 0; i < threads.size(); ++i) {
        cq.push([] { });
    }
}

void Threadpool::worker()
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
