
#include <iostream>
#include <mutex>
#include <chrono>
#include <thread>
#include <vector>
#include <sstream>
using namespace std::chrono_literals;
 
 std::mutex cout_mutex; // control access to std::cout
std::timed_mutex mutex;
 
void job(int id)
{
    std::ostringstream stream;
 
    for (int i = 0; i < 3; ++i) {
        if (mutex.try_lock_for(100ms)) {
            stream << "success ";
            std::this_thread::sleep_for(100ms);
            mutex.unlock();
        } else {
            stream << "failed ";
        }
        std::this_thread::sleep_for(100ms);
    }
 
    std::lock_guard<std::mutex> lock{cout_mutex};
    std::cout << "[" << id << "] " << stream.str() << "\n";
}
 
int main()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(job, i);
    }
 
    for (auto& i: threads) {
        i.join();
    }
}

/**
#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <mutex>
#include <algorithm>
#include <chrono>

class Vehicle
{
public:
    Vehicle(int id) : _id(id) {}
    int getID() { return _id; }

private:
    int _id;
};

class WaitingVehicles
{
public:
    WaitingVehicles() {}

    // getters / setters
    void printSize()
    {
        _mutex.lock();
        std::cout << "#vehicles = " << _vehicles.size() << std::endl;
        _mutex.unlock();
    }

    // typical behaviour methods
    void pushBack(Vehicle &&v)
    {
        std::lock_guard<std::mutex> lk(_big_lock);
        for (size_t i = 0; i < 1; ++i)
        {
            if (_mutex.try_lock_for(std::chrono::milliseconds(100)))
            {
                _vehicles.emplace_back(std::move(v));
                //std::this_thread::sleep_for(std::chrono::milliseconds(10));
                _mutex.unlock();
                break;
            }
            else
            {
                std::cout << "Error! Vehicle #" << v.getID() << " could not be added to the vector" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        } 
    }

private:
    std::vector<Vehicle> _vehicles; // list of all vehicles waiting to enter this intersection
    std::timed_mutex _mutex;
    std::mutex _big_lock;
};

int main()
{
    std::shared_ptr<WaitingVehicles> queue(new WaitingVehicles);
    std::vector<std::future<void>> futures;
    for (int i = 0; i < 1; ++i)
    {
        Vehicle v(i);
        futures.emplace_back(std::async(std::launch::async, &WaitingVehicles::pushBack, queue, std::move(v)));
    }

    std::for_each(futures.begin(), futures.end(), [](std::future<void> &ftr)
                  { ftr.wait(); });

    queue->printSize();

    return 0;
}

**/