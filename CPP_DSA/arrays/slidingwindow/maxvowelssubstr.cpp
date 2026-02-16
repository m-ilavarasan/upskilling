#include<iostream>
using namespace std;

int maxVowelsInSubstring(const string& str, int k)
{
     // This function is not used in this implementation
     for(int i = 0; i <= str.length() - k; i++)
     {
         int vCount = 0;
         for(int j = i; j < i + k; j++)
         {
             char c = str[j];
             if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
             {
                 vCount++;
             }
         }
         if(vCount > 0) return vCount;
     }
    return 0;
}
int main()
{

     string str = "abciidef"; 
     int k = 3;

     // Use bitmask instead of unordered_set for O(1) lookup
     bool isVowel[256] = {false};
     isVowel['a'] = isVowel['e'] = isVowel['i'] = isVowel['o'] = isVowel['u'] = true;
    
     int vCount = 0;
     int maxCount = 0;

     // Count vowels in first k characters
     for(int i = 0; i < k; i++)
     {
        if(isVowel[str[i]])
        {
            vCount++;
        }
     }
     
     maxCount = vCount;
     
     // Slide the window - O(n)
     for(int i = k; i < str.length(); i++)
     {
        // Remove leftmost character and add new character
        vCount -= isVowel[str[i - k]] ? 1 : 0;
        vCount += isVowel[str[i]] ? 1 : 0;
        maxCount = max(maxCount, vCount);
     }
       
     cout << "Maximum vowels in substring of size " << k << ": " << maxCount << endl;
     return 0;
}