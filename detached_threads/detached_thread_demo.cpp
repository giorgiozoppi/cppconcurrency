/**
 * Barron finishes cooking while Olivia cleans
 */
#include <thread>
#include <chrono>
#include <fmt/core.h>

void kitchen_cleaner() {
    while (true) {
        fmt::print("Olivia cleaned the kitchen.\n");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    std::thread olivia(kitchen_cleaner);
    olivia.detach();
    for (int i=0; i<3; i++) {
        fmt::print("Barron is cooking...\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }
    fmt::print("Barron is done!\n");
    // NOTE: detached thread are not joinable, if they were joinable you will get std::terminate.
}