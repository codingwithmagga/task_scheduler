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
        val += i % 2;
    }

    prom.set_value(val);
}

auto runTask(const int numThreads, const long long endSum)
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

    pool.quit();

    return duration;
}

void printCompTimeTable(const std::vector<std::pair<int, std::chrono::milliseconds>>& compTime)
{
    if (compTime.empty())
        return;

    std::cout << std::setw(10) << "Threads"
              << std::setw(15) << "Time (ms)"
              << std::setw(15) << "Speedup\n";
    std::cout << std::string(40, '-') << "\n";

    const auto single_thread_time = compTime[0].second;

    for (const auto& [threads, duration] : compTime) {
        auto time_ms = duration.count();
        // Calculate speedup for each entry (relative to single-threaded time)
        double speedup = (double)single_thread_time.count() / time_ms;

        std::cout << std::setw(10) << threads
                  << std::setw(15) << time_ms
                  << std::setw(15) << std::fixed << std::setprecision(2) << speedup << "\n";
    }
}

int main()
{
    const long long endSum = 1'000'000'000;
    std::vector<std::pair<int, std::chrono::milliseconds>> compTime;

    compTime.push_back({ 1, runTask(1, endSum) });

    for (int numThreads = 2; numThreads < 13; numThreads += 2) {
        compTime.push_back({ numThreads, runTask(numThreads, endSum) });
    }

    printCompTimeTable(compTime);

    return 0;
}
