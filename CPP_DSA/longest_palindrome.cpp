#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// Approach 1: Expand Around Center - O(n²) time, O(1) space
class LongestPalindrome {
private:
    // Helper function to expand around center and return palindrome length
    int expandAroundCenter(const string& s, int left, int right) {
        while (left >= 0 && right < s.length() && s[left] == s[right]) {
            left--;
            right++;
        }
        // Return length of palindrome found
        return right - left - 1;
    }

public:
    // Find longest palindromic substring using expand around center approach
    string longestPalindrome(const string& s) {
        if (s.empty()) return "";
        if (s.length() == 1) return s;

        int start = 0;
        int maxLen = 0;

        // Check all possible centers
        for (int i = 0; i < s.length(); i++) {
            // Case 1: Odd length palindromes (single character center)
            int len1 = expandAroundCenter(s, i, i);
            
            // Case 2: Even length palindromes (between two characters)
            int len2 = expandAroundCenter(s, i, i + 1);

            int len = max(len1, len2);

            // Update if we found a longer palindrome
            if (len > maxLen) {
                maxLen = len;
                start = i - (len - 1) / 2;  // Calculate start position
            }
        }

        return s.substr(start, maxLen);
    }
};

// Approach 2: Dynamic Programming - O(n²) time, O(n²) space
class LongestPalindromeDP {
public:
    string longestPalindrome(const string& s) {
        if (s.empty()) return "";
        
        int n = s.length();
        
        // dp[i][j] = true if substring from i to j is palindrome
        vector<vector<bool>> dp(n, vector<bool>(n, false));
        
        int start = 0;
        int maxLen = 1;

        // Single character is always palindrome
        for (int i = 0; i < n; i++) {
            dp[i][i] = true;
        }

        // Check for palindromes of length 2
        for (int i = 0; i < n - 1; i++) {
            if (s[i] == s[i + 1]) {
                dp[i][i + 1] = true;
                start = i;
                maxLen = 2;
            }
        }

        // Check for palindromes of length 3 and more
        for (int len = 3; len <= n; len++) {
            for (int i = 0; i < n - len + 1; i++) {
                int j = i + len - 1;

                // Check if s[i..j] is palindrome
                if (s[i] == s[j] && dp[i + 1][j - 1]) {
                    dp[i][j] = true;
                    start = i;
                    maxLen = len;
                }
            }
        }

        return s.substr(start, maxLen);
    }
};

// Approach 3: Brute Force - O(n³) time (simple but slow)
class LongestPalindromeBrute {
private:
    bool isPalindrome(const string& s, int start, int end) {
        while (start < end) {
            if (s[start] != s[end]) return false;
            start++;
            end--;
        }
        return true;
    }

public:
    string longestPalindrome(const string& s) {
        if (s.empty()) return "";

        string longest = "";

        // Check all substrings
        for (int i = 0; i < s.length(); i++) {
            for (int j = i; j < s.length(); j++) {
                if (isPalindrome(s, i, j)) {
                    int len = j - i + 1;
                    if (len > longest.length()) {
                        longest = s.substr(i, len);
                    }
                }
            }
        }

        return longest;
    }
};

int main() {
    cout << "========== Longest Palindromic Substring ==========" << endl << endl;

    // Test cases
    vector<string> testCases = {
        "babad",           // "bab" or "aba"
        "cbbd",            // "bb"
        "a",               // "a"
        "ac",              // "a" or "c"
        "racecar",         // "racecar"
        "forgeeksskeegfor" // "geeksskeeg"
    };

    cout << "--- Approach 1: Expand Around Center (Best) ---" << endl;
    cout << "Time: O(n²), Space: O(1)" << endl << endl;
    
    LongestPalindrome solver1;
    for (const auto& test : testCases) {
        string result = solver1.longestPalindrome(test);
        cout << "Input: \"" << test << "\" => Output: \"" << result << "\"" << endl;
    }

    cout << endl << "--- Approach 2: Dynamic Programming ---" << endl;
    cout << "Time: O(n²), Space: O(n²)" << endl << endl;
    
    LongestPalindromeDP solver2;
    for (const auto& test : testCases) {
        string result = solver2.longestPalindrome(test);
        cout << "Input: \"" << test << "\" => Output: \"" << result << "\"" << endl;
    }

    cout << endl << "--- Approach 3: Brute Force (Simple) ---" << endl;
    cout << "Time: O(n³), Space: O(1)" << endl << endl;
    
    LongestPalindromeBrute solver3;
    for (const auto& test : testCases) {
        string result = solver3.longestPalindrome(test);
        cout << "Input: \"" << test << "\" => Output: \"" << result << "\"" << endl;
    }

    cout << endl << "========== Complexity Comparison ==========" << endl;
    cout << "1. Expand Around Center: O(n²) time, O(1) space - BEST CHOICE" << endl;
    cout << "2. Dynamic Programming: O(n²) time, O(n²) space - High space" << endl;
    cout << "3. Brute Force:         O(n³) time, O(1) space - Too slow" << endl;
    cout << "4. Manacher's Algorithm: O(n) time, O(n) space - Complex to implement" << endl;

    cout << endl << "========== How Expand Around Center Works ==========" << endl;
    cout << "1. For each position i, treat it as center of palindrome" << endl;
    cout << "2. Expand left and right while characters match" << endl;
    cout << "3. Handle both odd-length (single char center)" << endl;
    cout << "   and even-length palindromes (two char center)" << endl;
    cout << "4. Track the longest palindrome found" << endl;

    return 0;
}
