#include <iostream>
#include <thread>
#include <future>
#include <cmath>
#include <random>
void computeSqrt(std::promise<double> &&prms, double input)
{
    std::random_device r;

    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(1, 2000);

    std::this_thread::sleep_for(std::chrono::milliseconds(uniform_dist(e1))); // simulate work
    double output = sqrt(input);
    prms.set_value(output);
}

int main()
{
    // define input data
    double inputData = 42.0;
    // Choose a random mean between 1 and 6

    // create promise and future
    std::promise<double> prms;
    std::future<double> ftr = prms.get_future();

    // start thread and pass promise as argument
    std::thread t(computeSqrt, std::move(prms), inputData);

    // Student task STARTS here
    // wait for result to become available
    auto status = ftr.wait_for(std::chrono::milliseconds(1000));
    if (status == std::future_status::ready) // result is ready
    {
        std::cout << "Result = " << ftr.get() << std::endl;
    }

    //  timeout has expired or function has not yet been started
    else if (status == std::future_status::timeout || status == std::future_status::deferred)
    {
        std::cout << "Result unavailable" << std::endl;
    }
    // Student task ENDS here

    // thread barrier
    t.join();

    return 0;
}
