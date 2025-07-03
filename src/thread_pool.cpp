#include "thread_pool.h"

Threadpool::Threadpool(const int numCores)
{
    for (int i = 0; i < numCores; ++i) {
        threads.push_back(std::thread(&Threadpool::worker, this));
    }
}
