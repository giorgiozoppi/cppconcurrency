#include <mutex>
#include <thread>
#include <memory>
#include <iterator>
#include <algorithm>
#include <vector>
#include <chrono>
#include <iostream>
using namespace std::chrono_literals;

using file_container = std::vector<std::string>;

class observant
{
public:
    virtual void update(file_container &&container) = 0;
};

class papermanager
{
public:
    papermanager(const file_container &c) : _paperlist(c)
    {
    }
    file_container get_list()
    {
        std::lock_guard<std::mutex> lg(_papermutex);
        std::cout << "Retrieving list" << std::endl;
        return _paperlist;
    }
    void add_new_file(const std::string &name)
    {
        std::lock_guard<std::mutex> lg(_papermutex);
        std::cout << "Add new file" << std::endl;
        _paperlist.push_back(name);
        for (auto client : _observer)
        {
            file_container c(_paperlist);
            std::cout << "Update from listeners" << std::endl;
            client->update(std::move(c));
        }
    }
    void add_observer(std::shared_ptr<observant> o)
    {
        _observer.push_back(o);
    }
    void update(file_container &&update_from_disk)
    {
        std::lock_guard<std::mutex> lg(_papermutex);
        _paperlist = std::move(update_from_disk);
        for (auto client : _observer)
        {
            file_container c(_paperlist);
            client->update(std::move(c));
        }
    }

private:
    std::vector<std::shared_ptr<observant>> _observer;
    file_container _paperlist;
    std::mutex _papermutex;
    std::mutex _observer_mutex;
};
void retrieve_from_disk(file_container &v)
{
    std::vector<std::string> sample{"/home/sam/vim.media",
                                    "/home/sam/digital.media",
                                    "/home/sam/rigid.media"};
    std::copy(begin(sample), end(sample), std::back_insert_iterator<std::vector<std::string>>(v));
}

class ui_observant : public observant
{
public:
    ui_observant(std::shared_ptr<papermanager> manager) : _manager(manager)
    {
    }
    void update(file_container &&files) override
    {
        std::lock_guard<std::mutex> lg(_observer_mutex);
        use(std::move(files));
    }
    void use(file_container &&files)
    {
        // do something
        for(auto& f: files) {
            std::cout << "Updated files:" << std::endl;
            std::cout << f << std::endl;
        }
    }

private:
    std::shared_ptr<papermanager> _manager;
    std::mutex _observer_mutex;
};

// main

int main()
{
    file_container files;
    retrieve_from_disk(files);
    auto paper_manager = std::make_shared<papermanager>(files);
    std::shared_ptr<observant> observant = std::make_shared<ui_observant>(paper_manager);
    // now we add a one more file.
    paper_manager->add_observer(observant);
    std::thread update_thread([&paper_manager]
                              {
                                  for (int i = 0; i < 5; i++)
                                  {
                                      std::string base{"file_"};
                                      base = base + std::to_string(i);
                                      std::cout << "Detected a new file " << base << std::endl;
                                      //  std::this_thread::sleep_for(std::chrono::seconds(1));
                                      paper_manager->add_new_file(base);
                                  }
                              });
    std::thread ui_thread([&observant]
                          {
                              for (int i = 0; i < 10; i++)
                              {
                                  // simulating UI event loop.
                                  std::this_thread::sleep_for(10ms);
                              }
                              std::cout << "Exiting UI" << std::endl;
                          });
    update_thread.join();
    ui_thread.join();
    std::cout << "Finished!" << std::endl;
}