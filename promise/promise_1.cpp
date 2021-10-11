/**
 * In order to achieve this, the threads need to adhere to a strict synchronization protocol. 
 * There is a such a mechanism available in the C++ standard that we can use for this purpose. 
 * This mechanism acts as a single-use channel between the threads. 
 * The sending end of the channel is called "promise" while the receiving end is called "future". 
 * In the C++ standard, the class template std::promise provides a convenient way 
 * to store a value or an exception that will acquired asynchronously at a later time via a std::future object. 
 * Each std::promise object is meant to be used only a single time.
 **/
#include <iostream>
#include <thread>
#include <future>

void modifyMessage(std::promise<std::string> && prms, std::string message)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // simulate work
    std::string modifiedMessage = message + " has been modified"; 
    prms.set_value(modifiedMessage);
}

int main(int argc, char** argv)
{
    // define message
    std::string messageToThread = "My Message";

    // create promise and future
    std::promise<std::string> prms;
    std::future<std::string> ftr = prms.get_future();

    // start thread and pass promise as argument
    std::thread t(modifyMessage, std::move(prms), messageToThread);

    // print original message to console
    std::cout << "Original message from main(): " << messageToThread << std::endl;

    // retrieve modified message via future and print to console
    std::string messageFromThread = ftr.get();
    std::cout << "Modified message from thread(): " << messageFromThread << std::endl;

    // thread barrier
    t.join();

    return 0;
}
