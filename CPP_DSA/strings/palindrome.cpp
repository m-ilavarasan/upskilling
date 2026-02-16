#include<iostream>
#include<string>

using namespace std;

int main()
{
    string pal = "madame";
    int left = 0, right = pal.length() - 1;
    
    while(left < right)
    {
        if(pal[left] != pal[right])
        {
            cout << "is not a palindrome\n";
            return 0;
        }
        left++;
        right--;
    }
    cout << "Yes it is a palindrome\n";
    return 0;
}