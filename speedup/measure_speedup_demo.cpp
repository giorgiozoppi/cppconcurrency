/**
 * Measure the speedup of a parallel algorithm
 */

#include <execution>
#include <numeric>
#include <future>
#include <chrono>
#include <vector>
#include <algorithm>

unsigned long long sequential_sum(unsigned int lo , unsigned int hi) {
    unsigned long long sum = 0;
    for (auto i=lo; i<=hi; i++) {
        sum += i;
    }
    return sum;
}

unsigned long long parallel_sum(unsigned int lo , unsigned int hi, unsigned int depth=0) {
    if (depth > 3) { // base case threshold
        unsigned long long sum = 0;
        for (auto i=lo; i<=hi; i++) {
            sum += i;
        }
        return sum;
    } else {  // divide and conquer
        auto mid = ((hi + lo) / 2); // middle index for splitting
        auto left = std::async(std::launch::async, parallel_sum, lo, mid, depth+1);
        auto right = parallel_sum(mid+1, hi, depth+1);
        return left.get() + right;
    }
}

int main() {
    const int NUM_EVAL_RUNS = 10;
    const int SUM_VALUE = 10000;
    int base = 0;

    //fmt::print("Evaluating Sequential Implementation...\n");
    std::chrono::duration<double> sequential_time(0);
    auto sequential_result = sequential_sum(0, SUM_VALUE); // "warm up"
    for (int i=0; i<NUM_EVAL_RUNS; i++) {
        auto start_time = std::chrono::high_resolution_clock::now();
        sequential_sum(0, SUM_VALUE);   
        sequential_time += std::chrono::high_resolution_clock::now() - start_time;
    }
    sequential_time /= NUM_EVAL_RUNS;
	std::chrono::duration<double> parallel_time(0);
    auto parallel_result = parallel_sum(0, SUM_VALUE); // "warm up"
    for (int i=0; i<NUM_EVAL_RUNS; i++) {
        auto start_time = std::chrono::high_resolution_clock::now();
        parallel_sum(0, SUM_VALUE);   
        parallel_time += std::chrono::high_resolution_clock::now() - start_time;
    }
    parallel_time /= NUM_EVAL_RUNS;  
    std::vector<int> v(SUM_VALUE);
    std::generate(v.begin(), v.end(), [&base] {
        base++;
        return base;
    });
    std::chrono::duration<double> standard_parallel_time(0);
    auto reduce_result = std::reduce(std::execution::par, std::begin(v), std::end(v)); // "warm up"
    for (int i=0; i<NUM_EVAL_RUNS; i++) {
        auto start_time = std::chrono::high_resolution_clock::now();
        std::reduce(std::execution::par, std::begin(v), std::end(v)) ;  
        standard_parallel_time += std::chrono::high_resolution_clock::now() - start_time;
    }
    standard_parallel_time /= NUM_EVAL_RUNS;


    // display sequential and parallel results for comparison
   printf("Sequential Result = %lld\nParallel Result = %lld\nC++17 ReduceResult = %d\n", sequential_result, parallel_result, reduce_result);
   printf("Average Sequential Time: %.1f ms\n", sequential_time.count()*1000);
   printf("Average Parallel Time: %.1f ms\n", parallel_time.count()*1000);
   printf("Average Reduce C++17 Parallel Time: %.1f ms\n", standard_parallel_time.count()*1000);
   printf("Speedup: %.2f\n", sequential_time/parallel_time);
   printf("C++17 Speedup: %.2f\n", sequential_time/standard_parallel_time);
   printf("Efficiency %.2f%%\n", 100*(sequential_time/parallel_time)/std::thread::hardware_concurrency());
   printf("C++17 Efficiency %.2f%%\n", 100*(sequential_time/standard_parallel_time)/std::thread::hardware_concurrency());

}