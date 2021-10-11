C++11 provides three locking strategies to be used when std::unique_lock or std::lock_guard wrapper is created.
- std::adopt_lock
- std::defer_lock
- std::try_to_lock
In some situations, the thread needs to hold two locks simultaneously and release them after accessing the shared data. 
When a thread has more than one lock, there is a chance of deadlock. To avoid this C++11 provides the first two strategies.
- std::adopt_lock assumes that the calling thread already owns the lock. The wrapper should adopt the ownership of the mutex and release it when control goes out of scope.
```c++
std::lock(m1, m2); // calling thread locks the mutex  
std::lock_guard<std::mutex> lock1(m1, std::adopt_lock);    
std::lock_guard<std::mutex> lock2(m2, std::adopt_lock);
// access shared data protected by the m1 and m2
```
- std::defer_lock doesn’t acquire ownership of the mutex and assumes that the calling thread is going to call lock to acquire the mutex. The wrapper is going to release the lock when control goes out of scope.
```c++
std::unique_lock<std::mutex> lock1(m1, std::defer_lock);    
std::unique_lock<std::mutex> lock2(m2, std::defer_lock);    
std::lock(lock1, lock2);
// access shared data protected by the m1 and m2
```
Difference between std::lock_guard and std::unique_lock
- std::lock_guard with std::adopt_lock strategy assumes the mutex is already acquired.
- std::unique_lock with std::defer_lock strategy assumes the mutex is not acquired on construction, rather than explicitly going to be locked.
