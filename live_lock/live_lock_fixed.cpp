/**
 * Two shoppers adding garlic and potatoes to a shared notepad
 * Work in progress.
 */
#include <thread>
#include <mutex>
#include <fmt/core.h>

#include <mutex>
#include <thread>
#include <chrono>
#include <memory>
#include <iostream>
#include <atomic>
using std::cout;

struct dinner;
std::atomic<int> limit{10};


template <typename T> class spoon  {
    public:
    spoon() {} 
    spoon(std::shared_ptr<T> d) : _owner(d) {}
    std::shared_ptr<T> owner() const { return _owner; }
    void set_owner(std::shared_ptr<T> d) {
        std::lock_guard<std::mutex> lk(_mutex);
        _owner = d;
    }
    void use() {
        std::lock_guard<std::mutex> lk(_mutex);
        std::cout << "I am using the spoon" << std::endl;         
    }

private:
    std::shared_ptr<T> _owner;
    std::mutex _mutex;
};
class dinner {
    public: 
    dinner(const std::string& name): _name(name), _is_hungry(true) {}
    std::string name() { return _name;}
    bool is_hungry() { return _is_hungry; }
    void eat_with(std::shared_ptr<spoon<dinner>> spoon,  std::shared_ptr<dinner> spouse) {
        while (_is_hungry) {
                // Don't have the spoon, so wait patiently for spouse.
                if (spoon->owner().get() != this) {
                    // sleep.
                    std::cout << this->name() << ": Hey " << spoon->owner()->name() << "! I am taking a siesta" << std::endl;
                    std::this_thread::sleep_for (std::chrono::seconds(2));
                    continue;
                }                       

                /*
                if (spouse->is_hungry() && ) {   
                    std::cout << this->name() << ": Please Eat first my darling " << spouse->name() << std::endl;                  
                    spoon->set_owner(spouse);
                    continue;
                }
                */

                // Spouse wasn't hungry, so finally eat
                spoon->use();
                _is_hungry = false;
                std::cout << name() << ": I am stuffed, my darling " << spouse->name() << std::endl;
                spoon->set_owner(spouse);
            }
    }
    private:
    std::string _name;
    std::atomic<bool> _is_hungry;
};
int main() {
    std::shared_ptr<dinner> husband = std::make_shared<dinner>("Bob");
    std::shared_ptr<dinner> wife = std::make_shared<dinner>("Alice");
    std::shared_ptr<spoon<dinner>> s = std::make_shared<spoon<dinner>>(husband);
    auto husband_eat = [](std::shared_ptr<dinner> husband,  
                     std::shared_ptr<dinner> wife,
                     std::shared_ptr<spoon<dinner>> spoon) { husband->eat_with(spoon, wife);};
    auto wife_eat = [](std::shared_ptr<dinner> husband,  
                     std::shared_ptr<dinner> wife,
                     std::shared_ptr<spoon<dinner>> spoon) { wife->eat_with(spoon, husband);};

    std::thread t1(husband_eat, husband, wife, s);
    std::thread t2(wife_eat, husband, wife, s);
    t1.join();
    t2.join(); 
}

