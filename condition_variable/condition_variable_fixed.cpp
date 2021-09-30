/**
 * Two hungry threads, anxiously waiting for their turn to take soup
 */
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <fmt/core.h>

std::atomic_int32_t soup_servings{10};
std::mutex slow_cooker_lid;
std::condition_variable soup_taken;

void hungry_person(int id) {
    int put_lid_back = 0;
  // pick up the slow cooker lid
    while (soup_servings > 0) {
        std::unique_lock<std::mutex> lid_lock(slow_cooker_lid); 
        while ((id != soup_servings % 5) && (soup_servings > 0)) { // is it your turn to take soup?
            put_lid_back++; // it's not your turn; put the lid back...

            /**
             * A spurious wakeup happens when a thread wakes up from waiting on a condition variable 
             * that's been signaled, only to discover that the condition 
             * it was waiting for isn't satisfied. It's called spurious 
             * because the thread has seemingly been awakened for no reason. 
             * But spurious wakeups don't happen for no reason: they usually happen because, 
             * in between the time when the condition variable was signaled 
             * and when the waiting thread finally ran, another thread ran and changed the condition. 
             * There was a race condition between the threads, 
             * with the typical result that sometimes, 
             * the thread waking up on the condition variable runs first, winning the race, 
             * and sometimes it runs second, losing the race. */
            
            soup_taken.wait(lid_lock); // ...and wait...
        }
        if (soup_servings > 0) {
            /**
             * The notifying thread does not need to hold the lock on the same mutex as the one held by 
             * the waiting thread(s); 
             * in fact doing so is a pessimization, 
             * since the notified thread would immediately block again, 
             * waiting for the notifying thread to release the lock.
             */
            soup_servings--; // it's your turn; take some soup!
            lid_lock.unlock(); // put back the lid
            soup_taken.notify_all(); // notify another thread to take their turn
             
        }
    }
    fmt::print("Person {} put the lid back {} times.\n", id, put_lid_back);
}

int main() {
    std::jthread hungry_threads[5];
    for (int i=0; i<5; i++) {
        hungry_threads[i] = std::jthread(hungry_person, i);
    }
    /*
    for (auto& ht : hungry_threads) {
        ht.join();
    }*/

}