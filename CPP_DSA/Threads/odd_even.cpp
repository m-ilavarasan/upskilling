#include<iostream>
#include<thread>
#include<chrono>
using namespace std;

void findEvenSum(int start , int end)
{
    int evenSum = 0;

    for(int i = start; i<= end; i++)
    {
        if((i&1) ==0)
            evenSum+=i;
    }
    cout <<" The even Sum is:" << evenSum<<endl;
}
void findOddSum(int start , int end)
{
    int oddSum = 0;

    for(int i = start; i<= end; i++)
    {
        if((i & 1) == 1 )
            oddSum+=i;
    }
    cout <<" The odd Sum is:" << oddSum <<endl;
}
int main()
{

    int start = 0; 
    int end = 1000000;
    auto startTime = chrono::high_resolution_clock::now();
    std::thread t1(findEvenSum,start, end);
    std::thread t2(findOddSum,start, end);
    t1.join();
    t2.join();
    //findEvenSum(start, end);
    //findOddSum(start, end);
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - startTime);
    cout << "Time taken: " << duration.count() << " ms" << endl;
    return 0;
}