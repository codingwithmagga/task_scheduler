#include <chrono>
#include <future>
#include <iostream>
#include <random>
#include <vector>

#include "thread_pool.h"
#include "visualizer.h"

using namespace std;
using namespace std::literals::chrono_literals;

const int MATRIX_SIZE = 1024;

void multiplyMatrixPart(const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    vector<vector<double>>& C,
    int startRow,
    int endRow,
    promise<void>&& prom)
{
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    prom.set_value();
}

vector<vector<double>> generateRandomMatrix()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> distrib(0.0, 1.0);

    vector<vector<double>> matrix(MATRIX_SIZE, vector<double>(MATRIX_SIZE));
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            matrix[i][j] = distrib(gen);
        }
    }
    return matrix;
}

auto runMatrixMultiplication(const int numThreads)
{
    Threadpool pool(numThreads);

    // Generate random matrices
    auto A = generateRandomMatrix();
    auto B = generateRandomMatrix();
    vector<vector<double>> C(MATRIX_SIZE, vector<double>(MATRIX_SIZE));

    auto start = chrono::high_resolution_clock::now();

    vector<future<void>> futures;
    int rowsPerThread = MATRIX_SIZE / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startRow = i * rowsPerThread;
        int endRow = (i == numThreads - 1) ? MATRIX_SIZE : startRow + rowsPerThread;

        promise<void> prom;
        futures.push_back(prom.get_future());

        pool.submit(multiplyMatrixPart, cref(A), cref(B), ref(C),
            startRow, endRow, std::move(prom));
    }

    // Wait for all threads to complete
    for (auto& future : futures) {
        future.get();
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    pool.quit();

    return duration;
}

int main()
{
    vector<pair<int, chrono::milliseconds>> compTime;

    compTime.push_back({ 1, runMatrixMultiplication(1) });

    for (int numThreads = 2; numThreads < 13; numThreads += 2) {
        compTime.push_back({ numThreads, runMatrixMultiplication(numThreads) });
    }

    cout << endl;
    Visualizer::printSpeedUpTable(compTime);

    return 0;
}
