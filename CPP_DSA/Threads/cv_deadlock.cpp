#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::mutex mtx;
std::condition_variable cv;

bool ready = false;
bool shutdown_flag = false;

void worker() {
    std::unique_lock<std::mutex> lock(mtx);

    std::cout << "Worker waiting...\n";

    cv.wait(lock, [] {
        std::cout << ready << " " << shutdown_flag << std::endl;
        return ready || shutdown_flag;
    });

    if (shutdown_flag) {
        std::cout << "Worker shutting down\n";
        return;
    }

    std::cout << "Worker processing\n";
}

int main() {
    std::thread t(worker);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lock(mtx);
        shutdown_flag = true;
    }
    cv.notify_all();
    t.join();

    std::cout << "Main exiting\n";
}
