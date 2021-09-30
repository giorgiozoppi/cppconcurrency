/**
 * Several users reading a calendar, but only a few users updating it
 * a shared_mutex is rwlock.
 * reading threads >> writing
 * - shared mode
 * - exclusive mode.
 */
#include <thread>
#include <mutex>
#include <chrono>
#include <shared_mutex>
#include <fmt/core.h>
#include <semaphore>
#include <vector>

constexpr int MAX_BUFFER = 10;

class buffer
{
public:
    buffer() {}
    void put(int k)
    {

        fmt::print("Waiting at PUT acquire\n");
        _spaces.acquire();
        fmt::print("PUT - Acquired {}\n", _in_idx);
        _slot[_in_idx++] = k;
        _elements.release();
    }
    int get()
    {
        int local{0};
        fmt::print("Waiting at GET acquire\n");
        _elements.acquire();
        fmt::print("GET - Acquired {}\n", _in_idx);
        local = _slot[_in_idx - 1];
        _in_idx--;
        _in_idx = _in_idx <= 0 ? 0 : _in_idx;
        _spaces.release();
        return local;
    }

private:
    int _slot[MAX_BUFFER];
    int _in_idx{0}, _out_idx{0};
    std::counting_semaphore<MAX_BUFFER> _spaces{10};
    std::counting_semaphore<MAX_BUFFER> _elements{0};
};
int main(int argc, char **argv)
{
    // create ten reader threads ...but only two writer threads
    buffer buf;
    std::vector<std::jthread> readers;
    std::vector<std::jthread> writers;
    for (auto i = 0; i < 20; i++)
    {
        readers.emplace_back(std::jthread([&buf]
                       { 
                           fmt::print("Value recieved: {}\n", buf.get()); 
                        }));

        writers.emplace_back(std::jthread([&buf]
                       {
                           auto v = 10 * 2;
                           buf.put(v);
                       }));
    }
}