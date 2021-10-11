#include <iostream>
#include <thread>
#include <future>
#include <cmath>
#include <vector>
#include <chrono>

void workerFunction(int n)
{
    // print system id of worker thread
    std::cout << "Worker thread id = " << std::this_thread::get_id() << std::endl;

    // perform work
    for (int i = 0; i < n; ++i)
    {
        sqrt(12345.6789);
    }
}

int main()
{
    // print system id of worker thread
    std::cout << "Main thread id = " << std::this_thread::get_id() << std::endl;

    // start time measurement
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    // launch various tasks
    std::vector<std::future<void>> futures;
    int nLoops = 1e7 , nThreads = 2;

    /**
     * fenix@fenix-Precision-3240-Compact:~/projects/cppconcurrency/async$ ./async_3 
Main thread id = 140613811168064
Worker thread id = 140613771060992
Worker thread id = 140613762668288
Worker thread id = 140613754275584
Worker thread id = 140613745882880
Worker thread id = 140613737490176
Execution finished after 11487 microseconds
*/
/*
std::async::deferred.
fenix@fenix-Precision-3240-Compact:~/projects/cppconcurrency/async$ ./async_3 
Main thread id = 139885128135488
Worker thread id = 139885088335616
Worker thread id = 139885079942912
Worker thread id = 139885071550208
Worker thread id = 139885063157504
Worker thread id = 139885054764800
Execution finished after 12669 microseconds
*/
/* both
fenix@fenix-Precision-3240-Compact:~/projects/cppconcurrency/async$ ./async_3 
Main thread id = 140563359892288
Worker thread id = 140563319875328
Worker thread id = 140563311482624
Worker thread id = 140563303089920
Worker thread id = 140563294697216
Worker thread id = 140563286304512
Execution finished after 10737 microseconds
*/
   // int nLoops = 10, nThreads = 5;
    for (int i = 0; i < nThreads; ++i)
    {
        futures.emplace_back(std::async(std::launch::async, workerFunction, nLoops));
    }

    // wait for tasks to complete
    for (const std::future<void> &ftr : futures)
        ftr.wait();

    // stop time measurement and print execution time
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << "Execution finished after " << duration <<" microseconds" << std::endl;
    
    return 0;
}
