#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>

std::mutex m1;
std::mutex m2;
std::atomic<bool> running{true};

void task1() {
    //while (running) {
         std::scoped_lock lock(m1,m2);
          std::cout << "Task1 acquired both locks\n";
        // m1.lock();
         std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // if (m2.try_lock_for(std::chrono::milliseconds(10))) {
        //     std::cout << "Task1 acquired both locks\n";
        //     m2.unlock();
        //     m1.unlock();
        //     break;
        // } else {
        //     std::cout << "Task1 failed to acquire m2, retrying...\n";
        //     m1.unlock();
        // }
   // }
}

void task2() {
   // while (running) {
        std::scoped_lock lock(m1,m2);
         std::cout << "Task2 acquired both locks\n";
        // m2.lock();
         std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // if (m1.try_lock_for(std::chrono::milliseconds(10))) {
        //     std::cout << "Task2 acquired both locks\n";
        //     m1.unlock();
        //     m2.unlock();
        //     break;
        // } else {
        //     std::cout << "Task2 failed to acquire m1, retrying...\n";
        //     m2.unlock();
        // }
    //}
}

int main() {
    std::thread t1(task1);
    std::thread t2(task2);

    t1.join();
    t2.join();

    return 0;
}
