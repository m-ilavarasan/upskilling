#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

using namespace std;

int money;
std::mutex m1;
std::mutex cout_mutex;
std::condition_variable cv;
bool money_ready = false;
#define MAX_MONEY 10
queue<int> money_queue;
void addMoney(int val) {
    // std::lock_guard<std::mutex> lock(m1);
    // {
    //     std::lock_guard<std::mutex> cout_lock(cout_mutex);
    //     cout << "add Money called\n";
    // }
    // for (int i = 0; i < 10; i++) {
    //     money++;
    //     {
    //         std::lock_guard<std::mutex> cout_lock(cout_mutex);
    //         cout << money << endl;
    //     }
    // }
   // money_ready = true;
    unique_lock<std::mutex> lock(m1);

    cv.wait(lock, [] { return money_queue.size() < MAX_MONEY; });
    money_queue.push(val);
    cout << "producing  " << val << endl; // Output the consumed value
    cout << "Buffer size after producing: " << money_queue.size() << endl << endl; // Display buffer size after consuming
    lock.unlock();
    cv.notify_one();
}

void deductMoney() {
    // Wait until producer has added money
    // std::unique_lock<std::mutex> lock(m1);
    // cv.wait(lock, [] { return money_ready; });
    // // Consume items one-by-one, updating the shared `money` each time.
    // while (true) {
    //     if (money == 0) break;
    //     --money;                     // update shared variable under lock
    //     int remaining = money;       // snapshot to print after unlocking
    //     lock.unlock();               // release mutex before printing
    //     {
    //         std::lock_guard<std::mutex> cout_lock(cout_mutex);
    //         cout << "deducted, remaining: " << remaining << '\n';
    //     }
    //     lock.lock();                 // reacquire lock for next iteration
    // }
    std::unique_lock<mutex> lock(m1);
    cv.wait(lock, [] {return money_queue.size()>0;});
    int val = money_queue.front();
    money_queue.pop();
        cout << "consuming  " << val << endl; // Output the consumed value
        cout << "Buffer size after consuming: " << money_queue.size() << endl << endl;
        lock.unlock();
    cv.notify_one();
}

int main() {
    money = 0;
    // std::thread t1(addMoney);
    // std::thread t2(deductMoney);

    std::thread producer([]{
        for (int i = 0; i < 20; i++) {
            addMoney(i);
        }
    });
    std::thread consumer([]{
        for (int i = 0; i < 20; i++) {
            deductMoney();
        }
    });

    producer.join();
    consumer.join();
    return 0;
}