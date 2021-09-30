/**
* 
* This code is an example of race condition.
* The interleaving between the two threads lead to an 
* unpredictable result/
*
*/
#include <thread>
#include <random>
#include <map>
#include <mutex>
#include <fmt/core.h>
constexpr int TIMES =50;
using shopping_list = std::map<std::string, uint64_t>;
std::mutex global_mutex;

void print_list(const shopping_list &l)
{
    for (const auto &[key, value] : l)
    {
        fmt::print("Key : {}, Value: {}\n", key, value);
    }
}
void shopper_1(shopping_list &list)
{

    for (int i = 0; i < TIMES; i++)
    {
        {
            std::scoped_lock m(global_mutex);

            if (list.find("apple") != list.end())
            {
                auto v = list["apple"];
                if (v == 0)
                {
                    v = 16;
                }
                v /= 4;
                list["apple"] = v;
            }
            else
            {
                list.insert({"apple", 3});
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
void shopper_2(shopping_list &list)
{

    for (int i = 0; i < TIMES; i++)
    {
        {
            std::scoped_lock m(global_mutex);
            if (list.find("apple") != list.end())
            {
                auto v = list["apple"];
                v *= 5;
                list["apple"] = v;
            }
            else
            {
                list.insert({"apple", 1});
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main()
{
    shopping_list shop_list;
    const auto processor_count = std::thread::hardware_concurrency();
    fmt::print("Number of cores {}\n", processor_count);
    std::vector<std::jthread> threads;
    for (size_t v = 0; v < processor_count; v++)
    {
        threads.push_back(std::jthread([&shop_list]
                                       { shopper_2(shop_list); }));
        threads.push_back(std::jthread([&shop_list]
                                       { shopper_1(shop_list); }));
    }
    threads.clear();
    print_list(shop_list);
}