/**
 * Two philosophers, thinking and eating sushi. Solved without deadlock because C++17
 * std::scoped_lock has a algorithm deadlock avoidance.
 */
#include <thread>
#include <mutex>
#include <fmt/format.h>

int sushi_count = 5000;


void philosopher(std::mutex &chopsticks) {
    std::scoped_lock scoped(chopsticks);
    while (sushi_count > 0) {
        if (sushi_count) {
            sushi_count--;
            return;
        }
    }
}

int main() {
    std::mutex chopsticks;
    std::thread barron(philosopher, std::ref(chopsticks));
    std::thread olivia(philosopher, std::ref(chopsticks));
    barron.join();
    olivia.join();
    fmt::print("The philosophers are done eating.\n");
}
