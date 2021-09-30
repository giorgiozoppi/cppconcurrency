/**
 * Two philosophers, thinking and eating sushi. Generate deadlock, a lock priority order.
 * also there is data race (write after read.)
 */
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <fmt/core.h>


class ServingLine
{
public:
    void serve_soup(int i)
    {
        {
            std::unique_lock<std::mutex> monitor(_line_lock);
            _queue.push(i);
        }
        _soup_served.notify_one();
    }
    int take_soup()
    {
        std::unique_lock<std::mutex> monitor(_line_lock);
        while(_queue.empty()) {
            _soup_served.wait(monitor);
        }
        auto value = _queue.front();
        _queue.pop();
        return value;
    }

private:
    std::mutex _line_lock;
    std::condition_variable _soup_served;
    std::queue<int> _queue;
};

int main()
{
    ServingLine line;
    std::thread consumer([&line]{
        for (auto i = 0; i < 10; i++) {
            fmt::print("Taking soup {}\n", line.take_soup());
        }
    });
    std::thread producer([&line]{
        for (auto i = 0; i < 10; i++) {
            fmt::print("Produce food {}\n", i);
     
            line.serve_soup(i);
        }
    });
    consumer.join();
    producer.join();
}