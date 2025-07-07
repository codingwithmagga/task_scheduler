# Task Scheduler

A multithreaded task scheduler implemented in C++23, featuring a thread pool and a lock-based concurrent queue. The project includes benchmarking examples for simple summation, Monte Carlo estimation of Pi, and matrix multiplication, with speedup visualization.

## Features

- **Thread Pool**: Efficiently manages a pool of worker threads for parallel task execution
- **Concurrent Queue**: Thread-safe priority queue for task scheduling
- **Speedup Visualization**: Prints tables comparing execution time and speedup for different thread counts
- **Examples**:
  - Benchmarking Simple summation ([examples/simple/main.cpp](examples/simple/main.cpp))
  - Benchmarking Monte Carlo Pi estimation ([examples/monte_carlo_pi/main.cpp](examples/monte_carlo_pi/main.cpp))
  - Benchmarking Matrix multiplication ([examples/matrix_multiplication/main.cpp](examples/matrix_multiplication/main.cpp))
  - Priority Tasks ([examples/priority_tasks/main.cpp](examples/priority_tasks/main.cpp))

## Building

This project uses CMake (version 3.25 or newer) and requires a C++23-compatible compiler.

```sh
git clone https://github.com/codingwithmagga/task_scheduler
cd task_scheduler
cmake -B build -S .
cmake --build build
```

Executables for the examples will be built in the `build/examples/` subdirectories.

## Running Examples

After building, run any of the example executables:

```sh
# Simple summation example
./build/examples/simple/simple_example

# Monte Carlo Pi estimation
./build/examples/monte_carlo_pi/monte_carlo_pi

# Matrix multiplication
./build/examples/matrix_multiplication/matrix_multiplication

# Priority Tasks
./build/examples/priority_tasks/priority_tasks
```

Each of the first example prints a table showing the speedup achieved with different thread counts which looks like this:

```bash
   Threads      Time (ms)       Speedup
----------------------------------------
         1           1642           1.00
         2            887           1.85
         4            516           3.18
         6            417           3.94
         8            350           4.69
        10            314           5.23
        12            300           5.47
```

Note that the matrix multiplication example does not show the expected speedup. This issue could be caused by different reasons, but I assume inefficient memory use and caching issues is a problem in this example.

The last example shows which tasks are executed first, based on their priority

```bash
Run task with priority: HIGH
Run task with priority: HIGH
Run task with priority: HIGH
Run task with priority: HIGH
Run task with priority: HIGH
Run task with priority: MEDIUM
Run task with priority: MEDIUM
Run task with priority: MEDIUM
Run task with priority: LOW
Run task with priority: LOW
```

## License

This project is licensed under the GNU General Public License v3.0. See [LICENSE](LICENSE) for details.
