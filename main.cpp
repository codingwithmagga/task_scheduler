#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>

using namespace std;
using namespace std::literals::chrono_literals;

mutex coutMut;

template <typename T>
class ConcurrentQueue {

public:
    void push(const T& val)
    {
        unique_lock lck(mut);

        if (q.size() >= max) {
            unique_lock lckCout(coutMut);
            cout << "Waiting cause q is full" << endl;
            lckCout.unlock();
            check_full.wait(lck, [this]() { return q.size() < max; });
        }

        q.push(val);
        check_empty.notify_all();
    }

    T front()
    {
        unique_lock lck(mut);

        if (q.empty()) {
            check_empty.wait(lck, [this]() { return !q.empty(); });
        }

        T val = q.front();
        q.pop();
        check_full.notify_all();

        return val;
    }

private:
    queue<T> q;
    mutex mut;
    size_t max = 20;

    condition_variable check_empty;
    condition_variable check_full;
};

void test() { }

using Func = std::function<void()>;
class Threadpool {

public:
    Threadpool(const int numCores)
    {
        for (int i = 0; i < numCores; ++i) {
            threads.push_back(thread(&Threadpool::worker, this));
        }

        cout << "Pool created with " << numCores << " cores" << endl;
    }

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

    vector<thread> threads;
    ConcurrentQueue<Func> cq;
    atomic<bool> running = true;
};

void taskSplit(const long long start, const long long end, std::promise<long long>&& prom)
{
    unique_lock lckCout(coutMut);
    std::cout << "Computation with " << start << "->" << end << std::endl;
    lckCout.unlock();

    long long val = 0;
    for (long long i = start; i < end; ++i) {
        val += 1;
    }

    prom.set_value(val);
}

int main()
{
    const int endSum = 1e9;
    {
        Threadpool pool(1);

        auto start = std::chrono::high_resolution_clock::now();

        std::promise<long long> prom;
        std::future<long long> fut = prom.get_future();

        pool.submit(taskSplit, 0, endSum, std::move(prom));

        int result = fut.get();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Result: " << result << std::endl;
        std::cout << "Execution time: " << duration.count() << " ms" << std::endl;

        pool.quit();
    }

    std::cout << "\n----------------------------------------------\n\n";

    {
        const int numThreads = 4;
        Threadpool pool(numThreads);

        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::future<long long>> futures;

        for (long long i = 0; i < numThreads; ++i) {
            std::promise<long long> prom;
            futures.push_back(prom.get_future());

            pool.submit(taskSplit, i * (endSum / numThreads), (i + 1) * (endSum / numThreads), std::move(prom));
        }

        long long result = 0;
        for (auto& future : futures) {
            result += future.get();
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Result: " << result << std::endl;
        std::cout << "Execution time: " << duration.count() << " ms" << std::endl;

        pool.quit();
    }

    std::cout << "\n----------------------------------------------\n\n";

    {
        const int numThreads = 8;
        Threadpool pool(numThreads);

        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::future<long long>> futures;

        for (long long i = 0; i < numThreads; ++i) {
            std::promise<long long> prom;
            futures.push_back(prom.get_future());

            pool.submit(taskSplit, i * (endSum / numThreads), (i + 1) * (endSum / numThreads), std::move(prom));
        }

        long long result = 0;
        for (auto& future : futures) {
            result += future.get();
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Result: " << result << std::endl;
        std::cout << "Execution time: " << duration.count() << " ms" << std::endl;

        pool.quit();
    }

    return 0;
}
