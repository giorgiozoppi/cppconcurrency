/**
 * Two shoppers adding garlic and potatoes to a shared notepad
 */
#include <thread>
#include <mutex>
#include <atomic>
#include <fmt/core.h>

std::mutex fib_k;
// replacw with
// std::recursive_mutex fib_k;

std::atomic_int32_t number;

int fib(int n)
{
    // error
    std::lock_guard<std::mutex> g(fib_k);
    // replace with
    // std::lock_guard<std::recursive_mutex> g(fib_k);
    if (n <= 1)
        return n;
    return fib(n-1) + fib(n-2);
}
void scientist() {
    number = fib(10);
}

int main() {
    std::thread barron(scientist);
    std::thread olivia(scientist);
    barron.join();
    olivia.join();
    fmt::print("Fibonacci of 10th {}\n", number);
}