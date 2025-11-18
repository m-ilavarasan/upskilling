#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<string> strs = {"flower", "flow", "flight"};
    if (strs.empty())
    {
        cout << "" << endl;
        return 0;
    }

    string prefix = strs[0];
    for (int i = 1; i < strs.size(); i++)
    {
        cout <<"string now " <<strs[i]<< endl;
        cout << "Current prefix before while: " << prefix << endl;
        int pos = strs[i].find(prefix);
        cout <<" pos :" <<  pos<<endl;
        while (strs[i].find(prefix) != 0)
        {
            prefix = prefix.substr(0, prefix.length() - 1);
            cout << "Current prefix: " << prefix << endl;
            if (prefix.empty())
            {
                cout << "" << endl;
                return 0;
            }
        }
    }
    cout << "final prefix:" << prefix << endl;
    return 0;
}