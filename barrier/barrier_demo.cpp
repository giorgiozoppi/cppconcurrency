/**
 * Deciding how many bags of chips to buy for the party
 */
#include <thread>
#include <mutex>
#include <fmt/format.h>

unsigned int bags_of_chips = 1; // start with one on the list
std::mutex pencil;

void cpu_work(unsigned long workUnits) {
    unsigned long x = 0;
    for (unsigned long i; i<workUnits*1000000; i++) {
        x++;
    }
}

void barron_shopper() {
    cpu_work(1); // do a bit of work first
    std::scoped_lock<std::mutex> lock(pencil);
    bags_of_chips *= 2;
    fmt::print("Barron DOUBLED the bags of chips.\n");
}

void olivia_shopper() {
    cpu_work(1); // do a bit of work first
    std::scoped_lock<std::mutex> lock(pencil);
    bags_of_chips += 3;
    fmt::print("Olivia ADDED 3 bags of chips.\n");
}

int main() {
    std::thread shoppers[10];
    for (int i=0; i<10; i+=2) {
        shoppers[i] = std::thread(barron_shopper);
        shoppers[i+1] = std::thread(olivia_shopper);
    }
    for (auto& s : shoppers) {
        s.join();
    }
    fmt::print("We need to buy {} bags_of_chips.\n", bags_of_chips);
}