#include "visualizer.h"

#include <iostream>

void Visualizer::printSpeedUpTable(const std::vector<std::pair<int, std::chrono::milliseconds>>& comp_time)
{
    if (comp_time.empty())
        return;

    std::cout << std::setw(10) << "Threads"
              << std::setw(15) << "Time (ms)"
              << std::setw(15) << "Speedup\n";
    std::cout << std::string(40, '-') << "\n";

    const auto single_thread_time = comp_time[0].second;

    for (const auto& [threads, duration] : comp_time) {
        auto time_ms = duration.count();
        // Calculate speedup for each entry (relative to single-threaded time)
        double speedup = (double)single_thread_time.count() / time_ms;

        std::cout << std::setw(10) << threads
                  << std::setw(15) << time_ms
                  << std::setw(15) << std::fixed << std::setprecision(2) << speedup << "\n";
    }
}
