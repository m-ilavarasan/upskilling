#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Horizontal Scanning: Compare each string with prefix
string horizontalScanning(vector<string> strs)
{
    if (strs.empty())
        return "";

    string prefix = strs[0];
    for (int i = 1; i < strs.size(); i++)
    {
        cout << "String now: " << strs[i] << endl;
        cout << "Current prefix before while: " << prefix << endl;
        
        while (strs[i].find(prefix) != 0)
        {
            prefix = prefix.substr(0, prefix.length() - 1);
            cout << "Current prefix: " << prefix << endl;
            if (prefix.empty())
            {
                return "";
            }
        }
    }
    return prefix;
}

// Vertical Scanning: Compare character by character across all strings
string verticalScanning(vector<string> strs)
{
    if (strs.empty())
        return "";

    // Iterate through each character position
    for (int charIndex = 0; charIndex < strs[0].length(); charIndex++)
    {
        char currentChar = strs[0][charIndex];
        cout << "Checking character position " << charIndex << " ('" << currentChar << "')" << endl;
        
        // Compare this character across all strings
        for (int strIndex = 1; strIndex < strs.size(); strIndex++)
        {
            cout << "  String " << strIndex << ": " << strs[strIndex];
            
            // If character position doesn't exist or doesn't match, return prefix up to this point
            if (charIndex >= strs[strIndex].length() || strs[strIndex][charIndex] != currentChar)
            {
                cout << " -> Character mismatch! Stopping at position " << charIndex << endl;
                return strs[0].substr(0, charIndex);
            }
            cout << " -> Match!" << endl;
        }
        cout << endl;
    }
    
    // All characters matched, return the entire first string
    return strs[0];
}

int main()
{
    vector<string> strs = {"flower", "flow", "flight"};
    
    cout << "========== HORIZONTAL SCANNING ==========" << endl;
    string result1 = horizontalScanning(strs);
    cout << "Final prefix (Horizontal): " << result1 << endl;
    
    cout << "\n========== VERTICAL SCANNING ==========" << endl;
    string result2 = verticalScanning(strs);
    cout << "Final prefix (Vertical): " << result2 << endl;
    
    return 0;
}