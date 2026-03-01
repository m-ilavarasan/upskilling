#include<iostream>
#include<thread>
#include<mutex>
using namespace std;

//std::mutex mtx;
std::recursive_mutex mtx;

void inner() {
   //std::lock_guard<std::mutex> lock(mtx);
  
    std::cout << "Inner\n";
    //lock.unlock();
}

void outer() {
   std::lock_guard<recursive_mutex> lock(mtx);

    inner();  // deadlock!
}

int main()
{
    std::thread t1(inner)  ;
    std::thread t2(outer);
    t1.join();
    t2.join();

    return 0;
}