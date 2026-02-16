#include<iostream>
#include<string>
#include<unordered_map>
using namespace std;

// Method 1: Optimized single-pass approach
bool isAnagramOptimized(string str1, string str2)
{
    if(str1.length() != str2.length())
        return false;
    
    unordered_map<char, int> charCount;
    for(char c : str1) {
        charCount[c]++;
    }
    
    for(char c : str2) {
        if(charCount[c] <= 0)
            return false;
        charCount[c]--;
    }
    
    return true;
}

// Method 2: Frequency Counter approach (explicit two-map comparison)
bool isAnagramFrequencyCounter(string str1, string str2)
{
    if(str1.length() != str2.length())
        return false;
    
    // Build frequency map for first string
    unordered_map<char, int> freq1;
    for(char c : str1) {
        freq1[c]++;
    }
    
    // Build frequency map for second string
    unordered_map<char, int> freq2;
    for(char c : str2) {
        freq2[c]++;
    }
    
    // Compare both frequency maps
    return freq1 == freq2;
}

int main()
{
    string str1 = "listen";
    string str2 = "silent";
    
    cout << "Testing: \"" << str1 << "\" and \"" << str2 << "\"\n\n";
    
    // Method 1 - Optimized approach
    if(isAnagramOptimized(str1, str2)) {
        cout << "Method 1 (Optimized): Is an anagram\n";
    } else {
        cout << "Method 1 (Optimized): Not an anagram\n";
    }
    
    // Method 2 - Frequency Counter approach
    if(isAnagramFrequencyCounter(str1, str2)) {
        cout << "Method 2 (Frequency Counter): Is an anagram\n";
    } else {
        cout << "Method 2 (Frequency Counter): Not an anagram\n";
    }
    
    return 0;
}