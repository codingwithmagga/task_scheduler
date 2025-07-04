#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <chrono>
#include <vector>

class Visualizer {
public:
    static void printSpeedUpTable(const std::vector<std::pair<int, std::chrono::milliseconds>>& comp_time);
};

#endif // VISUALIZER_H
