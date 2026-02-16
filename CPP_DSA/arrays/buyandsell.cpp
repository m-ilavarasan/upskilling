#include<iostream>
#include <vector>

using namespace std;

int main()
{

    vector<int> stocks = {7,6,5,4,3,2};
// buy - should be min
//Sell - should be max
// sell date should be upcoming days.
//int profit = 0;
int buyRate = INT_MAX;
int maxProfit = 0;
int sellIndex = -1;  // Use -1 to indicate no valid transaction

for(int i = 0; i < stocks.size(); i++)
{
    if(stocks[i] < buyRate)
    {
        buyRate = stocks[i];
    }
    
    int currentProfit = stocks[i] - buyRate;
    if(currentProfit > maxProfit)
    {
        maxProfit = currentProfit;
        sellIndex = i;
    }
}

cout << "Buy Rate: " << buyRate << " | Sell Index: " << sellIndex 
     << " | Max Profit: " << maxProfit << endl;
    return 0;
}