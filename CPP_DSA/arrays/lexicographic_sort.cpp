#include<iostream>
#include<vector>
#include<string>
using namespace std;

// Bubble Sort - Ascending Lexicographic Order
void bubbleSortAscending(vector<string>& strings)
{
    int n = strings.size();
    for(int i = 0; i < n - 1; i++)
    {
        for(int j = 0; j < n - i - 1; j++)
        {
            if(strings[j] > strings[j + 1])
            {
                // Swap
                string temp = strings[j];
                strings[j] = strings[j + 1];
                strings[j + 1] = temp;
            }
        }
    }
}

// Bubble Sort - Descending Lexicographic Order
void bubbleSortDescending(vector<string>& strings)
{
    int n = strings.size();
    for(int i = 0; i < n - 1; i++)
    {
        for(int j = 0; j < n - i - 1; j++)
        {
            if(strings[j] < strings[j + 1])
            {
                // Swap
                string temp = strings[j];
                strings[j] = strings[j + 1];
                strings[j + 1] = temp;
            }
        }
    }
}

// Sort by length first, then lexicographically
void sortByLengthThenLexicographic(vector<string>& strings)
{
    int n = strings.size();
    for(int i = 0; i < n - 1; i++)
    {
        for(int j = 0; j < n - i - 1; j++)
        {
            bool shouldSwap = false;
            if(strings[j].length() > strings[j + 1].length())
            {
                shouldSwap = true;
            }
            else if(strings[j].length() == strings[j + 1].length() && strings[j] > strings[j + 1])
            {
                shouldSwap = true;
            }
            
            if(shouldSwap)
            {
                string temp = strings[j];
                strings[j] = strings[j + 1];
                strings[j + 1] = temp;
            }
        }
    }
}

// Convert character to lowercase
char toLower(char c)
{
    if(c >= 'A' && c <= 'Z')
        return c + 32;
    return c;
}

// Compare strings case-insensitively
int compareIgnoreCase(const string& a, const string& b)
{
    int minLen = (a.length() < b.length()) ? a.length() : b.length();
    
    for(int i = 0; i < minLen; i++)
    {
        char ca = toLower(a[i]);
        char cb = toLower(b[i]);
        
        if(ca != cb)
            return (ca > cb) ? 1 : -1;
    }
    
    if(a.length() != b.length())
        return (a.length() > b.length()) ? 1 : -1;
    
    return 0;  // Equal
}

// Case-insensitive sorting
void sortCaseInsensitive(vector<string>& strings)
{
    int n = strings.size();
    for(int i = 0; i < n - 1; i++)
    {
        for(int j = 0; j < n - i - 1; j++)
        {
            if(compareIgnoreCase(strings[j], strings[j + 1]) > 0)
            {
                string temp = strings[j];
                strings[j] = strings[j + 1];
                strings[j + 1] = temp;
            }
        }
    }
}

// Display vector
void displayStrings(const vector<string>& strings)
{
    for(const auto& s : strings)
    {
        cout << s << " ";
    }
    cout << endl;
}

int main()
{
    // Sorting 1: Ascending Lexicographic Order
    vector<string> strings = {"banana", "apple", "cherry", "date", "elderberry", "avocado"};
    
    cout << "Original Strings:" << endl;
    displayStrings(strings);
    cout << endl;
    
    bubbleSortAscending(strings);
    cout << "Sorted in Ascending Lexicographic Order:" << endl;
    displayStrings(strings);
    cout << endl;
    
    // Sorting 2: Descending Lexicographic Order
    vector<string> strings1 = {"banana", "apple", "cherry", "date", "elderberry", "avocado"};
    
    bubbleSortDescending(strings1);
    cout << "Sorted in Descending Lexicographic Order:" << endl;
    displayStrings(strings1);
    cout << endl;
    
    // Sorting 3: Sort by length first, then lexicographically
    vector<string> strings2 = {"banana", "apple", "cat", "zebra", "dog", "elephant"};
    
    cout << "Original Strings 2:" << endl;
    displayStrings(strings2);
    cout << endl;
    
    sortByLengthThenLexicographic(strings2);
    cout << "Sorted by Length First, Then Lexicographically:" << endl;
    displayStrings(strings2);
    cout << endl;
    
    // Sorting 4: Case-insensitive sorting
    vector<string> strings3 = {"Zebra", "apple", "Banana", "cherry", "APPLE"};
    
    cout << "Original Strings 3 (Case Mixed):" << endl;
    displayStrings(strings3);
    cout << endl;
    
    sortCaseInsensitive(strings3);
    cout << "Sorted Case-Insensitive:" << endl;
    displayStrings(strings3);
    cout << endl;
    
    return 0;
}
