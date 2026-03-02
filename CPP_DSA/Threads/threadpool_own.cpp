#include<iostream>
#include<thread>
#include<mutex>
#include<queue>
#include<condition_variable>
#include<functional>
#include<vector>
#include<chrono>
#include <stdexcept>
using namespace std;

class ThreadPool{

    private:
    mutex mtx;
    condition_variable cv;
    queue<function <void()>> tasks;
    vector<thread> th;
    bool stop;

    public:
        ThreadPool(int numOfThreads): stop(false) {
        
                for(int i = 0; i < numOfThreads; i++){
                    th.emplace_back([this] {
                                while(true){
                                    function<void()> task;
                                    {
                                        unique_lock<mutex> lock(mtx);
                        cv.wait(lock, [this]() {
                            return stop || !tasks.empty();
                        });

                        if (stop && tasks.empty())
                            return;

                        task = move(tasks.front());
                        tasks.pop();
                    }

                    // Protect worker thread from crashing
                    try {
                        task();
                    } catch (const exception& e) {
                        cerr << "Task exception: " << e.what() << endl;
                    } catch (...) {
                        cerr << "Unknown task exception\n";
                    }
                }
            });
        }
    }

    void enqueue(function<void()> task) {
        {
            unique_lock<mutex> lock(mtx);
            if (stop)
                throw runtime_error("Enqueue on stopped ThreadPool");
            
            tasks.emplace(move(task));
            }
            cv.notify_one();
        }
    ~ThreadPool() {
        {
           
            unique_lock<mutex> lock(mtx);
            stop = true;
        }
            cv.notify_all();
            
            for(auto &t: th){
            if (t.joinable())
                t.join();
            }
        }
};



int main()
{

    ThreadPool pool(5);
    for(int i = 0; i < 5; i++)
    {
        pool.enqueue([i]() {
            cout <<" Task:" <<i << " threadId:" << this_thread::get_id() <<endl;
            this_thread::sleep_for(chrono::milliseconds(100));
        });
    }
    return 0;
}