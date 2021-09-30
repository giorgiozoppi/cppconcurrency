/**
 * Two philosophers, thinking and eating sushi. Solved without deadlock because C++17
 * std::scoped_lock has a algorithm deadlock avoidance.
 */
#include <thread>
#include <mutex>

int sushi_count = 5000;


void philosopher(std::mutex& lock_a, std::mutex& lock_b) {
    std::scoped_lock lock(lock_a, lock_b);
    while (sushi_count > 0) {
        if (sushi_count) {
            sushi_count--;
        }
    }
}

int main() {
    std::mutex chopstick_a;
    std::mutex chopstick_b;
    std::thread barron(philosopher, std::ref(chopstick_a), std::ref(chopstick_b));
    std::thread olivia(philosopher, std::ref(chopstick_a), std::ref(chopstick_b));
    barron.join();
    olivia.join();
    printf("The philosophers are done eating.\n");
}