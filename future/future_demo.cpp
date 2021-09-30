/**
* Promise e future
 "You can make a Promise and it's up to you to keep it. 
  When someone else makes you a promise you must wait to see if they honour it in the Future".
  The result of the future will be set by "someone else" - by the result of an asynchronous computation.
  On the other hand, the result of a promise can be set by "you" (or in fact by anybody) anytime because it has a public setter method. 
  In C++ the situation is similar: the promise object is used by the producer and the future object by the consumer. 
  The advantage of this separation is that the client cannot set the value of the future and who set the promise to fulfill the future,
  can set when the data to provide is ready.
 */
#include <thread>
#include <mutex>
#include <boost/thread/future.hpp>
#include <fmt/core.h>
#include <future>

int how_many_vegetable() {
    fmt::format("Olivia is counting vegetables");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 42;
}

void chop_veggie(uint8_t i)
{
    auto id = std::hash<std::thread::id>()(std::this_thread::get_id());
    fmt::print("Chopping veggie for thread_id {}\n", id);
}

int main() {
    auto res = std::async(std::launch::deferred, how_many_vegetable);
    auto data = res.get();
    fmt::print("Got {}\n", data);
}
