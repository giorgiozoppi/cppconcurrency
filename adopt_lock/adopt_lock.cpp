#include <mutex>
#include <thread>
#include <iostream>


class BankAccount {
 public:
    explicit BankAccount(int balance): balance(balance) {}
    int getBalance() { return balance; }
    friend void transfer(BankAccount* fromAcc, BankAccount* toAcc, int amount);
 private:
    int balance;
    std::mutex m;
};

void transfer(BankAccount* fromAcc, BankAccount* toAcc, int amount) {
    // first two mutexes are locked
    std::lock(fromAcc->m, toAcc->m);

    // two lock_guard instances are constructed, one for each mutex
    // std::adopt_lock parameter specifies that mutexes are already
    // locked, and they should just adopt the ownership  of the existing
    // lock on the mutex rather than attempt to lock the mutex in the
    // constructor.
    std::lock_guard<std::mutex> lock1(fromAcc->m, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(toAcc->m, std::adopt_lock);

    fromAcc->balance -= amount;
    toAcc->balance += amount;
}

int main() {
    BankAccount person1(100);
    BankAccount person2(200);

    std::cout << "Balance person1: " << person1.getBalance() << std::endl;
    std::cout << "Balance person2: " << person2.getBalance() << std::endl;

    std::thread t1(transfer, &person1, &person2, 50);
    std::thread t2(transfer, &person2, &person1, 20);

    if (t1.joinable()) t1.join();
    if (t2.joinable()) t2.join();

    std::cout << "Balance person1 after transfer: " <<
        person1.getBalance() << std::endl;
    std::cout << "Balance person2 after transfer: " <<
        person2.getBalance() << std::endl;

    return 0;
}