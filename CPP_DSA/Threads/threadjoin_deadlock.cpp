#include <iostream>
#include <thread>
#include <chrono>

std::thread t1;
std::thread t2;

void thread1_func() {
    std::cout << "Thread 1 started\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "Thread 1 waiting for Thread 2\n";
    // if (t2.joinable())
    //     t2.join();  // 💥 Deadlock

    std::cout << "Thread 1 finished\n";
}

void thread2_func() {
    std::cout << "Thread 2 started\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "Thread 2 waiting for Thread 1\n";
    // if (t1.joinable())
    //     t1.join();  // 💥 Deadlock

    std::cout << "Thread 2 finished\n";
}

int main() {
    t1 = std::thread(thread1_func);
    t2 = std::thread(thread2_func);

    if (t1.joinable())
        t1.join();
    if (t2.joinable())
        t2.join();

    std::cout << "Main finished\n";
}
