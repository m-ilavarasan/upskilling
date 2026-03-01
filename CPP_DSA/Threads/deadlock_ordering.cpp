#include <iostream>
#include <thread>
#include <mutex>
using namespace std;
std::mutex mtxA;
std::mutex mtxB;

void function1() {
   // std::lock_guard<std::mutex> lockA(mtxA);
   scoped_lock lock(mtxA, mtxB);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Function1 done\n";
}

void function2() {
    //std::lock_guard<std::mutex> lockB(mtxB);
    scoped_lock lock(mtxA, mtxB);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Function2 done\n";
}

int main()
{
    std::thread t1(function1);
    std::thread t2(function2);
    t1.join();
    t2.join();

    return 0;
}