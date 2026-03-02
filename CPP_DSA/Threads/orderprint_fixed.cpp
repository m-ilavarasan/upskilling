#include<iostream>
#include<thread>
#include<mutex>
#include<vector>
#include<condition_variable>
using namespace std;

mutex mtx;
condition_variable cv;
int turn = 0;
const int noOfTimes = 5;
// //using cv.noify_one() and cv.wait() with predicate to print in order
// void PrintOrder_notifyone(char c, vector<char>& letters, int index)
// {

//     for(int i = 0; i< noOfTimes; i++)
//     {
//         cout << "Thread " << c << " waiting for its turn\n";
//         unique_lock<mutex> lock(mtx);
//         cv.wait(lock, [c, &letters, index] { return turn==index; });
//         cout << c;
//         turn = (turn + 1) % letters.size();
//        // lock.unlock();
//         cv.notify_one();
//     }
// }
void PrintOrder(char c, vector<char>& letters, int index)
{

    for(int i = 0; i< noOfTimes; i++)
    {
        cout << "Thread " << c << " waiting for its turn\n";
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [c, &letters, index] { return turn==index; });
        cout << c;
        turn = (turn + 1) % letters.size();
       // lock.unlock();
        cv.notify_all();
    }
}
int main()
{

    vector<char> letters = {'A','B','C','D','E'};
    vector<thread> threads;
    for(int i = 0; i< letters.size(); i++)
    {
        //threads.emplace_back(PrintOrder_notifyone, letters[i], ref(letters),i);
        threads.emplace_back(PrintOrder, letters[i], ref(letters),i);
    }  
    for(auto& t : threads)
    {
        t.join();
    }
    return 0;
}