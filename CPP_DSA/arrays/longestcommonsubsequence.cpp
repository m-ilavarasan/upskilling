#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Approach 1: Recursive LCS (Simple but inefficient)
int lcsRecursive(string text1, string text2, int m, int n)
{
    // Base case: if either string is empty
    if (m == 0 || n == 0)
        return 0;
    
    // If characters match, include this character in LCS and move both pointers
    if (text1[m - 1] == text2[n - 1])
    {
        cout << "Match found: '" << text1[m - 1] << "' at positions " << m - 1 << " and " << n - 1 << endl;
        return 1 + lcsRecursive(text1, text2, m - 1, n - 1);
    }
    
    // If characters don't match, find max of two cases:
    // 1. Exclude last character of text1
    // 2. Exclude last character of text2
    return max(lcsRecursive(text1, text2, m - 1, n),
               lcsRecursive(text1, text2, m, n - 1));
}

// Approach 2: Dynamic Programming (Memoization) - More efficient
int lcsDP(string text1, string text2)
{
    int m = text1.length();
    int n = text2.length();
    
    // Create 2D DP table
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    cout << "Building DP table:" << endl;
    
    // Fill the DP table
    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (text1[i - 1] == text2[j - 1])
            {
                // Characters match, add 1 to diagonal value
                dp[i][j] = dp[i - 1][j - 1] + 1;
                cout << "Match: text1[" << i - 1 << "]='" << text1[i - 1] 
                     << "' == text2[" << j - 1 << "]='" << text2[j - 1] 
                     << "' -> dp[" << i << "][" << j << "] = " << dp[i][j] << endl;
            }
            else
            {
                // Characters don't match, take max from left or top
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    cout << "\nDP Table:" << endl;
    cout << "    ";
    for (int j = 0; j < n; j++)
        cout << text2[j] << " ";
    cout << endl;
    
    for (int i = 0; i <= m; i++)
    {
        if (i > 0)
            cout << text1[i - 1] << " ";
        else
            cout << "  ";
        
        for (int j = 0; j <= n; j++)
            cout << dp[i][j] << " ";
        cout << endl;
    }
    
    return dp[m][n];
}

// Approach 3: Find actual LCS string (not just length)
string getLCSString(string text1, string text2)
{
    int m = text1.length();
    int n = text2.length();
    
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    // Fill DP table
    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (text1[i - 1] == text2[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            }
            else
            {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    // Backtrack to find the actual LCS string
    string lcs = "";
    int i = m, j = n;
    
    cout << "\nBacktracking to find LCS string:" << endl;
    while (i > 0 && j > 0)
    {
        if (text1[i - 1] == text2[j - 1])
        {
            lcs = text1[i - 1] + lcs;  // Add character to result
            cout << "Character '" << text1[i - 1] << "' added. LCS so far: " << lcs << endl;
            i--;
            j--;
        }
        else if (dp[i - 1][j] > dp[i][j - 1])
        {
            i--;  // Move up
            cout << "Move up in table" << endl;
        }
        else
        {
            j--;  // Move left
            cout << "Move left in table" << endl;
        }
    }
    
    return lcs;
}

int main()
{
    string text1 = "abcde";
    string text2 = "ace";
    
    cout << "=============== RECURSIVE APPROACH ===============" << endl;
    cout << "String 1: " << text1 << endl;
    cout << "String 2: " << text2 << endl;
    int result1 = lcsRecursive(text1, text2, text1.length(), text2.length());
    cout << "LCS Length (Recursive): " << result1 << endl;
    
    cout << "\n=============== DYNAMIC PROGRAMMING APPROACH ===============" << endl;
    cout << "String 1: " << text1 << endl;
    cout << "String 2: " << text2 << endl;
    int result2 = lcsDP(text1, text2);
    cout << "\nLCS Length (DP): " << result2 << endl;
    
    cout << "\n=============== GET ACTUAL LCS STRING ===============" << endl;
    cout << "String 1: " << text1 << endl;
    cout << "String 2: " << text2 << endl;
    string lcsString = getLCSString(text1, text2);
    cout << "\nActual LCS String: " << lcsString << endl;
    cout << "LCS Length: " << lcsString.length() << endl;
    
    // Test with another example
    cout << "\n=============== ANOTHER EXAMPLE ===============" << endl;
    string text3 = "AGGTAB";
    string text4 = "GXTXAYB";
    cout << "String 1: " << text3 << endl;
    cout << "String 2: " << text4 << endl;
    string lcsString2 = getLCSString(text3, text4);
    cout << "\nActual LCS String: " << lcsString2 << endl;
    cout << "LCS Length: " << lcsString2.length() << endl;
    
    return 0;
}
