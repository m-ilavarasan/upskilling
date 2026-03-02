#include<iostream>
#include<thread>

using namespace std;

class Singleton{
    private:
    Singleton(){};
    public:
   
    static Singleton& getInstance(){
       static Singleton instance;
        return instance;
    }
    Singleton(const Singleton&) = delete;
    void operator=(const Singleton&) = delete;
};

int main()
{
    Singleton& st = Singleton::getInstance();
    return 0;
}