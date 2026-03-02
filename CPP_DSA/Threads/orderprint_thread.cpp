#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
using namespace std;

mutex mtx;
condition_variable cv;
char initChar = 'A';
char lastChar = 'C';
const int noOfTimes = 5;

//Print ABCABCABCABCABC
void PrintOrder(char c)
{
    for(int i = 0; i< noOfTimes; i++)
    {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [c] { return c == initChar; });
        cout << c;
        initChar = (initChar == 'A') ? 'B' : (initChar == 'B') ? 'C' : 'A';
        lock.unlock();
        cv.notify_all();
    }
}


int main()
{
    thread tc (PrintOrder, 'C');
    thread tb(PrintOrder,'B');

    thread ta(PrintOrder, 'A');

     tc.join();
     tb.join();
     ta.join();
   
    return 0;
}