/**
 */
#include <thread>
#include <mutex>
#include <boost/asio.hpp>
#include <fmt/core.h>

void chop_veggie(uint8_t i)
{
    auto id = std::hash<std::thread::id>()(std::this_thread::get_id());
    fmt::print("Chopping veggie for thread_id {}\n", id);
}

int main() {
    boost::asio::thread_pool pool(20);
    for (auto i =0; i < 500; i++) {
        boost::asio::post(pool, [i]{
            chop_veggie(i);
        });
    }
    pool.join();

}