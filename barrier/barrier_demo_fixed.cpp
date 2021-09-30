/**
 * Deciding how many bags of chips to buy for the party. Note:
 A barrier provides a thread-coordination mechanism that allows an expected number of threads to block until 
the  same  number  of  threads  have  arrived  at  the  barrier.  The  expected  number  is  set  when  the  barrier  is 
constructed, but may change according to some functions' effects. Unlike latches, barriers are reusable; once 
threads are released from a barrier's synchronization point, they can reuse the same barrier immediately. [Note: 
It is thus useful for managing repeated tasks, or phases of a larger task, that are handled by multiple threads. — 
end note.]  
23 A barrier has a completion phase that is a (possibly empty) set of effects. When the member functions defined 
in this subclause arrive at the barrier, they have the following effects:  
1. When the expected number of threads has arrived at the barrier, one of the participating threads executes 
the barrier type's completion phase.  
2. When the completion phase is completed, all threads blocked at the synchronization point are unblocked. 
The end of the completion phase synchronizes with the returns from all calls unblocked by its completion. 
 */
#include <thread>
#include <mutex>
#include <barrier>
#include <fmt/core.h>
#include <atomic>
constexpr int BARRIER_ITEMS = 48;
std::int32_t bags_of_chips{1}; // start with one on the list
std::mutex pencil;
std::barrier fist_bump(BARRIER_ITEMS);

void cpu_work(unsigned long workUnits)
{
    unsigned long x = 0;
    for (unsigned long i; i < workUnits * 1000000; i++)
    {
        x++;
    }
}

void barron_shopper()
{
    cpu_work(1); // do a bit of work first
    fist_bump.arrive_and_wait();
    {
        std::scoped_lock lock(pencil);
        bags_of_chips *= 2;
    }
}

void olivia_shopper()
{
    cpu_work(1); // do a bit of work first
    {
        std::scoped_lock lock(pencil);
        bags_of_chips += 3;
    }
    fist_bump.arrive_and_wait();
}

int main()
{
    std::thread shoppers[BARRIER_ITEMS];
    for (int i = 0; i < BARRIER_ITEMS; i += 2)
    {
        shoppers[i] = std::thread(barron_shopper);
        shoppers[i + 1] = std::thread(olivia_shopper);
    }
    for (auto &s : shoppers)
    {
        s.join();
    }
    printf("We need to buy %u bags_of_chips.\n", bags_of_chips);
}
