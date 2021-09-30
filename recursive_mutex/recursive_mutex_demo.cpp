/**
 * Two shoppers adding garlic and potatoes to a shared notepad
 */
#include <thread>
#include <mutex>
#include <fmt/core.h>

unsigned int garlic_count = 0;
unsigned int potato_count = 0;
std::mutex pencil;

void add_garlic() {
    pencil.lock();
    garlic_count++;
    pencil.unlock();
}

void add_shopping() {
    pencil.lock();
    potato_count++;
    add_garlic();
    pencil.unlock();
}

void shopper() {
    for (int i=0; i<10000; i++) {
        add_shopping();
    }
}

int main() {
    std::thread barron(shopper);
    std::thread olivia(shopper);
    barron.join();
    olivia.join();
    fmt::print("We should buy {} garlic.\n", garlic_count);
    fmt::print("We should buy {} potatoes.\n", potato_count);
}