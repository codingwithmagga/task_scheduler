#include <chrono>
#include <iostream>
#include <mutex>
#include <random>

#include "thread_pool.h"

using namespace std::chrono_literals;

std::mutex coutMut;
std::atomic<int> counter = 0;

std::string PrioToString(const Priority priority)
{
    switch (priority) {
    case Priority::HIGH:
        return "HIGH";
    case Priority::MEDIUM:
        return "MEDIUM";
    case Priority::LOW:
        return "LOW";
    }
    return "unknown";
}

void runTask(const Priority priority)
{
    std::this_thread::sleep_for(200ms);
    std::lock_guard<std::mutex> lock(coutMut);
    std::cout << "Run task with priority: " << PrioToString(priority) << std::endl;
    ++counter;
}

int main()
{
    const int numExamples = 10;

    Threadpool pool(2);

    std::vector<Priority> priorities(numExamples);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 2);

    for (int i = 0; i < priorities.size(); ++i) {
        priorities[i] = static_cast<Priority>(distrib(gen));
    }

    for (auto& p : priorities) {
        pool.submitPrio(runTask, p, p);
    }

    while (counter < numExamples) {
        std::this_thread::sleep_for(100ms);
    }

    pool.quit();

    return 0;
}
