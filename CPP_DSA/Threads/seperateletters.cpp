#include<iostream>
#include<thread>
#include<mutex>
#include<vector>
#include<condition_variable>
#include<set>
#include<map>

using namespace std;

std::mutex m1;
std::condition_variable cv;
int turn = 0;
map<char, int> charOrder;

void segregate(vector<char> chars, char c)
{
    unique_lock<mutex> lock(m1);
    cv.wait(lock, [c]() { return turn == charOrder[c]; });
    
    for(int i = 0; i< chars.size(); i++)
    {
        if(chars[i] == c)
        {
            cout <<" The letter is : " << chars[i] <<endl;
        }
    }
    
    turn++;
    cv.notify_all();
}


int main()
{
    vector<char> chars = {'A', 'B', 'C' , 'B', 'C' , 'A' , 'A', 'C','B'};
    
    // Find unique characters dynamically
    set<char> uniqueChars(chars.begin(), chars.end());
    
    // Map each character to its order
    int order = 0;
    for(char c : uniqueChars)
    {
        charOrder[c] = order++;
    }
    
    // Create threads dynamically
    vector<thread> threads;
    for(char c : uniqueChars)
    {
        threads.push_back(thread(segregate, chars, c));
    }
    
    // Join all threads
    for(auto& t : threads)
    {
        t.join();
    }
    
    return 0;
}