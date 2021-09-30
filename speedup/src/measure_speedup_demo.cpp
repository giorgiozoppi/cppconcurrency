/**
 * Measure the speedup of a parallel algorithm
 */

#include <execution>
#include <numeric>
#include <future>
#include <chrono>
#include <fmt/core.h>
#include <vector>
#include <algorithm>

unsigned long long sequential_sum(unsigned int lo , unsigned int hi) {
    unsigned long long sum = 0;
    for (auto i=lo; i<hi; i++) {
        sum += i;
    }
    return sum;
}

unsigned long long parallel_sum(unsigned int lo , unsigned int hi, unsigned int depth=0) {
    if (depth > 3) { // base case threshold
        unsigned long long sum = 0;
        for (auto i=lo; i<hi; i++) {
            sum += i;
        }
        return sum;
    } else {  // divide and conquer
        auto mid = (hi + lo) / 2; // middle index for splitting
        auto left = std::async(std::launch::async, parallel_sum, lo, mid, depth+1);
        auto right = parallel_sum(mid, hi, depth+1);
        return left.get() + right;
    }
}

int main() {
    const int NUM_EVAL_RUNS = 10;
    const int SUM_VALUE = 1000000;

    fmt::print("Evaluating Sequential Implementation...\n");\
    std::chrono::duration<double> sequential_time(0);
    auto sequential_result = sequential_sum(0, SUM_VALUE); // "warm up"
    for (int i=0; i<NUM_EVAL_RUNS; i++) {
        auto start_time = std::chrono::high_resolution_clock::now();
        sequential_sum(0, SUM_VALUE);   
        sequential_time += std::chrono::high_resolution_clock::now() - start_time;
    }
    sequential_time /= NUM_EVAL_RUNS;
	
    fmt::print("Evaluating Parallel Implementation...\n");
    std::chrono::duration<double> parallel_time(0);
    auto parallel_result = parallel_sum(0, SUM_VALUE); // "warm up"
    for (int i=0; i<NUM_EVAL_RUNS; i++) {
        auto start_time = std::chrono::high_resolution_clock::now();
        parallel_sum(0, SUM_VALUE);   
        parallel_time += std::chrono::high_resolution_clock::now() - start_time;
    }
    parallel_time /= NUM_EVAL_RUNS;

    fmt::print("Evaluating Parallel Reduce C++17 Implementation...\n");
    std::chrono::duration<double> standard_parallel_time(0);
    std::vector<int> v;
    v.reserve(SUM_VALUE);
    std::iota(v.begin(), v.end(), 0);

    std::reduce(std::execution::par, std::begin(v), std::end(v)); // "warm up"
    for (int i=0; i<NUM_EVAL_RUNS; i++) {
        auto start_time = std::chrono::high_resolution_clock::now();
        std::reduce(std::execution::par, std::begin(v), std::end(v)) ;  
        standard_parallel_time += std::chrono::high_resolution_clock::now() - start_time;
    }
    standard_parallel_time /= NUM_EVAL_RUNS;


    // display sequential and parallel results for comparison
    if (sequential_result != parallel_result) {
        fmt::print("ERROR: Result mismatch!\n  Sequential Result = {}\n  Parallel Result = {}\n", sequential_result, parallel_result);
    }
    fmt::print("Average Sequential Time: %.1f ms\n", sequential_time.count()*1000);
    fmt::print("  Average Parallel Time: %.1f ms\n", parallel_time.count()*1000);
     fmt::print("  Average Reduce C++17 Parallel Time: %.1f ms\n", standard_parallel_time.count()*1000);
    fmt::print("Speedup: %.2f\n", sequential_time/parallel_time);
    fmt::print("C++17 Speedup: %.2f\n", sequential_time/standard_parallel_time);
    fmt::print("Efficiency %.2f%%\n", 100*(sequential_time/parallel_time)/std::thread::hardware_concurrency());
        fmt::print("C++17 Efficiency %.2f%%\n", 100*(sequential_time/standard_parallel_time)/std::thread::hardware_concurrency());

}