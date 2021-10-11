#include <iostream>
#include <thread>
#include <future>
#include <cmath>
#include <memory>
/**
 * The std::async (part of the <future> header) function template is used to start a (possibly) asynchronous task. 
 * It returns a std::future object, which will eventually hold the return value of std::async's parameter function.
 * When the value is needed, we call get() on the std::future instance; this blocks the thread 
 * until the future is ready and then returns the value. 
 * std::launch::async or std::launch::deferred can be specified as the first parameter 
 * to std::async in order to specify how the task is run.
 * std::launch::async indicates that the function call must be run on its own (new) thread. 
 * std::launch::deferred indicates that the function call is to be deferred until either wait() or get() is called on the future. 
 * Ownership of the future can be transferred to another thread before this happens.
 * std::launch::async | std::launch::deferred indicates that the implementation may choose. This is the default option (when you don't specify one yourself). 
 * It can decide to run synchronously.
 * 
 * So we can say:
 * std::async makes it possible to control the amount of concurrency by passing an optional launch parameter, 
 * which enforces either synchronous or asynchronous behavior. 
 * This ability, especially when left to the system, allows us to prevent an overload of threads, 
 * which would eventually slow down the system as threads consume resources 
 * for both management and communication. 
 * If we were to use too many threads, the increased resource consumption would outweigh 
 * the advantages of parallelism and slow down the program.
 *  Use async when you can :).
 */

double divideByNumber(double num, double denom)
{
    // print system id of worker thread
    std::cout << "Worker thread id = " << std::this_thread::get_id() << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate work

    if (denom == 0)
        throw std::runtime_error("Exception from thread#: Division by zero!");

    return num / denom;
}


int main()
{
    // print system id of worker thread
    std::cout << "Main thread id = " << std::this_thread::get_id() << std::endl;

    // use async to start a task
    double num = 42.0, denom = 2.0;


    std::future<double> ftr = std::async(std::launch::deferred, divideByNumber, num, denom);

    // retrieve result within try-catch-block
    try
    {
        double result = ftr.get();
        std::cout << "Result = " << result << std::endl;
    }
    catch (const std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}