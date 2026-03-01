#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <chrono>

using namespace std;

// Shared resources
queue<int> buffer;
std::mutex buffer_mutex;
std::mutex cout_mutex;
std::condition_variable cv;
const int MAX_BUFFER_SIZE = 5;
const int ITEMS_PER_PRODUCER = 5;
bool production_done = false;

// Producer function
void producer(int producer_id) {
    for (int i = 0; i < ITEMS_PER_PRODUCER; i++) {
        //int item = producer_id * 100 + i;  // Generate unique item
        
        std::unique_lock<std::mutex> lock(buffer_mutex);
        
        // Wait if buffer is full
        cv.wait(lock, [] { return buffer.size() < MAX_BUFFER_SIZE; });
        
        buffer.push(i);
        
        {
            std::lock_guard<std::mutex> cout_lock(cout_mutex);
            cout << "Producer " << producer_id << " produced: " << i 
                 << " | Buffer size: " << buffer.size() << endl;
        }
        
        lock.unlock();
        cv.notify_all();  // Notify all waiting consumers
        
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

// Consumer function
void consumer(int consumer_id) {
    while (true) {
        std::unique_lock<std::mutex> lock(buffer_mutex);
        
        // Wait if buffer is empty and production is not done
        cv.wait(lock, [] { return !buffer.empty() || production_done; });
        
        if (buffer.empty() && production_done) {
            lock.unlock();
            break;  // Exit if buffer is empty and all producers are done
        }
        
        if (!buffer.empty()) {
            int item = buffer.front();
            buffer.pop();
            
            {
                std::lock_guard<std::mutex> cout_lock(cout_mutex);
                cout << "Consumer " << consumer_id << " consumed: " << item 
                     << " | Buffer size: " << buffer.size() << endl;
            }
            
            lock.unlock();
            cv.notify_all();  // Notify producers waiting to produce
            
            this_thread::sleep_for(chrono::milliseconds(150));
        }
    }
}

int main() {
    const int NUM_PRODUCERS = 2;
    const int NUM_CONSUMERS = 2;
    
    cout << "=== Multi-Producer, Multi-Consumer Example ===" << endl;
    cout << "Producers: " << NUM_PRODUCERS << " | Consumers: " << NUM_CONSUMERS << endl;
    cout << "Max Buffer Size: " << MAX_BUFFER_SIZE << " | Items per Producer: " << ITEMS_PER_PRODUCER << endl << endl;
    
    // Create producer threads
    vector<std::thread> producers;
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        producers.push_back(std::thread(producer, i));
    }
    
    // Create consumer threads
    vector<std::thread> consumers;
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumers.push_back(std::thread(consumer, i));
    }
    
    // Wait for all producers to finish
    for (auto& p : producers) {
        p.join();
    }
    
    // Signal that production is done
    {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        production_done = true;
    }
    cv.notify_all();  // Wake up any sleeping consumers
    
    // Wait for all consumers to finish
    for (auto& c : consumers) {
        c.join();
    }
    
    cout << "\n=== Program Complete ===" << endl;
    cout << "Final buffer size: " << buffer.size() << endl;
    
    return 0;
}
