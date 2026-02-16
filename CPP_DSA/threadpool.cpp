#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <functional>
#include <chrono>

// Thread Pool implementation
class ThreadPool {
private:
    // Task is a function that takes no arguments and returns nothing
    using Task = std::function<void()>;

    std::vector<std::thread> workers;
    std::queue<Task> tasks;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool shutdown;

    // Worker thread function
    void workerThread() {
        while (true) {
            Task task;
            {
                // Lock and wait for a task
                std::unique_lock<std::mutex> lock(queueMutex);
                cv.wait(lock, [this] { return !tasks.empty() || shutdown; });

                // If shutdown and no tasks left, exit
                if (shutdown && tasks.empty()) {
                    break;
                }

                // Get task from queue
                if (!tasks.empty()) {
                    task = tasks.front();
                    tasks.pop();
                }
                else {
                    continue;
                }
            }

            // Execute task outside of lock
            if (task) {
                task();
            }
        }
    }

public:
    // Constructor - create thread pool with given number of threads
    ThreadPool(size_t numThreads) : shutdown(false) {
        std::cout << "Creating ThreadPool with " << numThreads << " threads..." << std::endl;
        
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] { workerThread(); });
        }
        
        std::cout << "ThreadPool created successfully!" << std::endl << std::endl;
    }

    // Destructor - shutdown the pool
    ~ThreadPool() {
        shutdown_pool();
    }

    // Submit a task to the thread pool
    template<typename F>
    void submit(F task) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (shutdown) {
                std::cerr << "Cannot submit task: ThreadPool is shutting down!" << std::endl;
                return;
            }
            tasks.emplace(task);
        }
        cv.notify_one();  // Notify one worker thread
    }

    // Get the number of pending tasks
    size_t pendingTasks() {
        std::unique_lock<std::mutex> lock(queueMutex);
        return tasks.size();
    }

    // Get the number of worker threads
    size_t getThreadCount() const {
        return workers.size();
    }

    // Gracefully shutdown the pool
    void shutdown_pool() {
        if (shutdown) return;  // Already shutdown

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            shutdown = true;
        }

        cv.notify_all();  // Wake up all threads

        // Wait for all threads to finish
        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }

        std::cout << "ThreadPool shutdown complete!" << std::endl;
    }

    // Wait until all tasks are completed
    void wait() {
        while (true) {
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                if (tasks.empty()) {
                    break;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
};

// Example usage
int main() {
    std::cout << "========== Simple ThreadPool Example ==========" << std::endl << std::endl;

    // Create a thread pool with 4 worker threads
    ThreadPool pool(4);

    // Example 1: Submit 10 simple tasks
    {
        std::cout << "--- Submitting 10 tasks to 4 threads ---" << std::endl;
        
        for (int i = 1; i <= 10; ++i) {
            pool.submit([i]() {
                std::cout << "Task " << i << " running on thread " 
                          << std::this_thread::get_id() << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            });
        }

        // Wait for all tasks to complete
        pool.wait();
        std::cout << "\nAll tasks completed!" << std::endl << std::endl;
    }

    // Example 2: Tasks with calculation
    {
        std::cout << "--- Tasks with calculations ---" << std::endl;
        
        for (int i = 1; i <= 4; ++i) {
            pool.submit([i]() {
                int sum = 0;
                for (int j = 1; j <= 10; ++j) {
                    sum += j;
                }
                std::cout << "Task " << i << " calculated sum: " << sum << std::endl;
            });
        }

        pool.wait();
        std::cout << "\nAll calculations completed!" << std::endl << std::endl;
    }

    std::cout << "========== How ThreadPool Works ==========" << std::endl;
    std::cout << "1. Create N worker threads (e.g., 4)" << std::endl;
    std::cout << "2. Each thread waits for tasks from a queue" << std::endl;
    std::cout << "3. Submit tasks: added to queue and notify waiting threads" << std::endl;
    std::cout << "4. Free threads pick up tasks and execute them" << std::endl;
    std::cout << "5. No task creation overhead (threads are reused)" << std::endl;
    std::cout << "=========================================" << std::endl;

    return 0;
}
