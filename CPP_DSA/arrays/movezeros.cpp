#include<iostream>
#include<vector>
using namespace std;


int main()
{

    vector<int> test = {1,0,3,0,2,2,0,5};

    int len = test.size();

    int j = 0;

    for(int i = 0;i<len; i++)
    {
        if(test[i] !=0)
        {
           if(i !=j)
           {
                swap(test[i], test[j]);
           }
           j++;
        }
    }

    for(int x= 0; x <len; x++)
    {
        cout<< test[x] <<"\t";
    }



    return 0;
}