#include <chrono>
#include <future>
#include <iostream>
#include <random>

#include "thread_pool.h"
#include "visualizer.h"

using namespace std;
using namespace std::literals::chrono_literals;

void valuesInCircle(const long long runs, std::promise<long long>&& prom)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(0, 1);

    long long valuesInCircle = 0;
    for (long long i = 0; i < runs; ++i) {
        double x = distrib(gen);
        double y = distrib(gen);

        if (x * x + y * y <= 1) {
            ++valuesInCircle;
        }
    }

    prom.set_value(valuesInCircle);
}

auto runTask(const int numThreads, const long long numRuns)
{
    Threadpool pool(numThreads);

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::future<long long>> futures;

    for (long long i = 0; i < numThreads; ++i) {
        std::promise<long long> prom;
        futures.push_back(prom.get_future());

        pool.submit(valuesInCircle, numRuns / numThreads, std::move(prom));
    }

    long long valuesInCircle = 0;
    for (auto& future : futures) {
        valuesInCircle += future.get();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Calculation for pi: " << 4.0 * valuesInCircle / (double)numRuns << std::endl;

    pool.quit();

    return duration;
}

int main()
{
    const long long numRuns = 100'000'000;
    std::vector<std::pair<int, std::chrono::milliseconds>> compTime;

    compTime.push_back({ 1, runTask(1, numRuns) });

    for (int numThreads = 2; numThreads < 13; numThreads += 2) {
        compTime.push_back({ numThreads, runTask(numThreads, numRuns) });
    }

    std::cout << std::endl;

    Visualizer::printSpeedUpTable(compTime);

    return 0;
}
