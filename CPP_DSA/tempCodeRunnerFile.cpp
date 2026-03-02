    unordered_map<int, int> consolidated;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return consolidated;
    }
    
    string line;

    while (getline(file, line)) {
        stringstream ss(line);

        int year, total;
        ss >> year >> total;
        cout << year << " " << total << endl;
        consolidated[year] = total;
    }