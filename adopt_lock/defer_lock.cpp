#include <mutex>
#include <thread>
#include <iostream>


class BankAccount {
 public:
    explicit BankAccount(int balance): balance(balance) {}
    int getBalance() { return balance; }
    friend void transfer( BankAccount* fromAcc, BankAccount* toAcc, int amount);
 private:
    int balance;
    std::mutex m;
};

void transfer(BankAccount* fromAcc, BankAccount* toAcc, int amount) {
    // std::unique_lock doesn't always own the mutex, it's associated 
    // with. The 2nd argument std::defer_lock indiactes that mutex should remain
    // unlocked on construction. The lock can then be acquired later by calling
    // lock() on std::unique_lock object. 
    std::unique_lock<std::mutex> lock1(fromAcc->m, std::defer_lock);
    std::unique_lock<std::mutex> lock2(toAcc->m, std::defer_lock);
    std::lock(lock1, lock2);

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