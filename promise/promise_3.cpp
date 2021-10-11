#include <iostream>
#include <thread>
#include <future>
#include <cmath>
#include <memory>

/**
 * The future-promise communication channel may also be used for passing exceptions. 
 * To do this, the worker thread simply sets an exception rather than a value in the promise. 
 * In the parent thread, the exception is then re-thrown once get() is called on the future.
 * In the thread function, we need to implement a try-catch block which can be set to 
 * catch a particular exception or - as in our case - to catch all exceptions. 
 * Instead of setting a value, we now want to throw a std::exception along with a customized error message. 
 * In the catch-block, we catch this exception and throw it to the parent thread using the promise 
 * with set_exception. 
 * The function std::current_exception allows us to easily retrieve the exception which has been thrown.
 * On the parent side, we now need to catch this exception. 
 * In order to do this, we can use a try-block around the call to get(). 
 * We can set the catch-block to catch all exceptions or - as in this example - we could also 
 * catch a particular one such as the standard exception. 
 * Calling the method what() on the exception allows us to retrieve the message from the exception 
 * - which is the one defined on the promise side of the communication channel. 
 * When we run the program, we can see that the exception is being thrown in the worker thread 
 * with the main thread printing the corresponding error message to the console. 
 * So a promise future pair can be used to pass either values or exceptions between threads.
 */
void divideByNumber(std::promise<double> &&prms, double num, double denom)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate work
    try
    {
        if (denom == 0)
            throw std::runtime_error("Exception from thread: Division by zero!");
        else
            prms.set_value(num / denom);
    }
    catch (...)
    {
        prms.set_exception(std::current_exception());
    }
}

int main()
{
    // create promise and future
    std::promise<double> prms;
    std::future<double> ftr = prms.get_future();

    // start thread and pass promise as argument
    double num = 42.0, denom = 0.0;
    std::thread t(divideByNumber, std::move(prms), num, denom);

    // retrieve result within try-catch-block
    try
    {
        double result = ftr.get();
        std::cout << "Result = " << result << std::endl;
    }
    catch (std::runtime_error& e)
    {
        std::cout << "Exception " << e.what() << std::endl;
    }

    // thread barrier
    t.join();

    return 0;
}
