#include <thread>
#include <mutex>
#include <future>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <cmath>
#include <vector>
#include <iostream>
#include <chrono>
using namespace std::chrono_literals;

std::mutex rmutex;

template <typename T, typename Result>
class active_object
{

public:
    active_object(std::function<Result(T)> &&f)
    {
        _thread = std::thread(&active_object::process, this, std::move(f));
        //  _thread.detach();
    }
    ~active_object()
    {
        stop();
        _thread.join();
    }
    active_object(const active_object &m) = delete;
    active_object &operator=(const active_object &m) = delete;
    active_object(active_object &&v)
    {
        _queue = std::move(v._queue);
        _queue_mutex = std::move(v._queue_mutex);
    }
    active_object &operator=(active_object &&v)
    {
        _queue = std::move(v._queue);
        _queue_mutex = std::move(v._queue_mutex);
    }
    void process(std::function<Result(T)> &&f)
    {
        std::pair<T, std::promise<Result>> msg;
        {
            std::unique_lock<std::mutex> lk(_queue_mutex);
            while (!_stop)
            {
                while (_queue.empty())
                {
                    cv.wait(lk);
                }
                if (!_queue.empty())
                {
                    msg = std::move(_queue.front());
                    _queue.pop();

                    Result res;
                    res = f(msg.first);

                    msg.second.set_value(res);
                }
            }
            while (!_queue.empty())
            {
                msg = std::move(_queue.front());
                _queue.pop();

                Result res;
                res = f(msg.first);

                msg.second.set_value(res);
            }
        }
    }
    void stop()
    {
        _stop = true;
        cv.notify_all();
    }
    auto send_message(T &&msg)
    {

        std::future<Result> res;
        std::promise<Result> p;
        T item(std::move(msg));
        res = std::move(p.get_future());
        {
            // std::cout << "Before queue mutex" << std::endl;
            std::lock_guard<std::mutex> lk(_queue_mutex);
            //  std::cout << "Adding to the queue" << std::endl;
            _queue.push({std::move(item), std::move(p)});
            //   std::cout << "Addded message to the queue" << std::endl;
        }

        cv.notify_one();
        return res;
    }

private:
    std::queue<std::pair<T, std::promise<Result>>> _queue;
    std::mutex _queue_mutex;
    std::function<Result(T)> processor;
    std::thread _thread;
    std::mutex _cond_var_mutex;
    std::condition_variable cv;
    bool _ready;
    std::atomic<bool> _stop{false};
};
int main(int argc, char **argv)
{
    std::vector<std::future<double>> results;
    {
        {
            active_object<double, double> obj([](double v)
                                              {
                                                  // simulating work.
                                                  std::this_thread::sleep_for(100ms);
                                                  double x{v};
                                                  return x + 2;
                                              });
            for (int i = 0; i < 200000; i++)
            {
                auto k = obj.send_message(double(i + 1));
                {
                    std::lock_guard<std::mutex> rg(rmutex);
                    results.emplace_back(std::move(k));
                }
            }
        }
        for (auto &&r : results)
        {
            auto v = r.get();
            std::cout << v << std::endl;
        }
    }
}