#include<iostream>
#include<sstream>
#include<fstream>
#include<unordered_map>

using namespace std;

bool validateData()
{
    return true;
}

unordered_map<int, int> getConsolidateDataSet(const string& filename) {
    unordered_map<int, int> consolidated;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return consolidated;
    }
    
    string line;
    getline(file, line);  // Skip header line

    while (getline(file, line)) {
        stringstream ss(line);
        string yearStr, totalStr;
        
        getline(ss, yearStr, ',');
        getline(ss, totalStr, ',');
        
        int year = stoi(yearStr);
        int total = stoi(totalStr);
        
        cout << year << " " << total << endl;
        consolidated[year] = total;
    }

    file.close();
    return consolidated;
}
unordered_map<int, int> getDataSet(const string& filename) {
    unordered_map<int, int> yearTotals;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return yearTotals;
    }
    
    string line;
    getline(file, line);  // Skip header line

    while (getline(file, line)) {
        stringstream ss(line);

        int year;
        string state;
        string month;
        int times;

        ss >> year >> state >> month >> times;
        cout << year << state <<month << times <<endl;
        yearTotals[year] += times;   // aggregate duplicates
    }

    file.close();
    return yearTotals;
}

int main()
{
    string file1 = "C:\\Ilavarasan\\Dataset2.csv";
    string file2 = "C:\\Ilavarasan\\DataSet1.csv";
   unordered_map<int, int> dataSet= getDataSet(file2);
   unordered_map<int, int> conDataSet= getConsolidateDataSet(file1);

    bool isCorrect = validateData();
    return 0;
}