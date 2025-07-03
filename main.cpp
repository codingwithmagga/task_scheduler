#include <chrono>
#include <future>
#include <iostream>

#include "thread_pool.h"

using namespace std;
using namespace std::literals::chrono_literals;

void taskSplit(const long long start, const long long end, std::promise<long long>&& prom)
{
    long long val = 0;
    for (long long i = start; i < end; ++i) {
        val += 1;
    }

    prom.set_value(val);
}

void runTask(const int numThreads, const int endSum)
{
    Threadpool pool(numThreads);

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::future<long long>> futures;

    for (long long i = 0; i < numThreads; ++i) {
        std::promise<long long> prom;
        futures.push_back(prom.get_future());

        if (i == numThreads - 1) {
            pool.submit(taskSplit, i * (endSum / numThreads), endSum, std::move(prom));
        } else {
            pool.submit(taskSplit, i * (endSum / numThreads), (i + 1) * (endSum / numThreads), std::move(prom));
        }
    }

    long long result = 0;
    for (auto& future : futures) {
        result += future.get();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Result for " << numThreads << " threads" << std::endl;
    std::cout << "Result: " << result << std::endl;
    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;
    std::cout << "\n----------------------------------------------\n\n";

    pool.quit();
}

int main()
{
    const int endSum = 1e9;

    runTask(1, endSum);

    for (int numThreads = 2; numThreads < 13; numThreads += 2) {
        runTask(numThreads, endSum);
    }

    return 0;
}
