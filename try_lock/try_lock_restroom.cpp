/**
 * Two shoppers adding items to a shared notepad
 */
#include <thread>
#include <mutex>
#include <chrono>
#include <fmt/format.h>
std::mutex heavens_handle;

void knock_the_door(const char *name)
{
    for (auto times = 0; times < 1000; ++times)
    {
        if (heavens_handle.try_lock())
        {
            fmt::print("{} can pee in Heaven's Restroom...cool!\n",name);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            heavens_handle.unlock();
        }
        else
        {
            fmt::print("{} waited for Heaven's Restroom opening\n", name);
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
        }
    }
}

int main()
{
    auto start_time = std::chrono::steady_clock::now();
    std::thread barron(knock_the_door, "Barron");
    std::thread olivia(knock_the_door, "Olivia");
    barron.join();
    olivia.join();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count();
    printf("Elapsed Time: %.2f seconds\n", elapsed_time / 1000.0);
}