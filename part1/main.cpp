#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

void book_to_string(string& s, const string& book) {
    s += ' ';
    s += '"';
    s += book;
    s += '"';
}

string assemble_string(const string& command, int& start, int& end, const string& bookStr) {
    string fullStr = command;
    fullStr += ' ';
    fullStr += to_string(start);
    fullStr += '-';
    fullStr += to_string(end);
    if (command != "Delete") {
        fullStr += ',';
        fullStr += bookStr;
    }
    return fullStr;
}

void search(
    ofstream& fout,
    vector<string>& original,
    vector<string>& desired, 
    vector<vector<int>>& dp, 
    vector<string>& op,
    vector<int>& index,
    vector<string>& result,
    int& minSize,
    int i, 
    int j, 
    bool& found) 
{
    string str;
    if (found) {
        return;
    }
    
    if (i == 0 && j == 0) {
        int numOp = op.size();
        if (numOp == 0) return;
        
        vector<string> temp;
        string bookStr;
        int currSize;
        
        string command = op[numOp - 1];
        int start = index[numOp - 1], end = index[numOp - 1];
        book_to_string(bookStr, desired[end]);
        
        for (int i = numOp - 2; i >= 0 ; --i) {
            if (op[i] == command && index[i] == end + 1) {
                end = index[i];
                book_to_string(bookStr, desired[end]);
                continue;
            }
            temp.push_back(assemble_string(command, start, end, bookStr));
            command = op[i];
            bookStr.clear();
            end = start = index[i];
            book_to_string(bookStr, desired[end]);
        }
        temp.push_back(assemble_string(command, start, end, bookStr));
        currSize = temp.size();
        //fout << currSize << endl;
        if (currSize < minSize) {
            result = temp;
            minSize = currSize;
        }
        
        //found = true;
        return;
    }
    
    if (i == 0) {
        op.push_back("Insert");
        index.push_back(j - 1);
        search(fout, original, desired, dp, op, index, result, minSize, i, j - 1, found);
        op.pop_back();
        index.pop_back();
        return;
    }
    
    if (j == 0) {
        op.push_back("Delete");
        index.push_back(i - 1);
        search(fout, original, desired, dp, op, index, result, minSize, i - 1, j, found);
        op.pop_back();
        index.pop_back();
        return;
    }

    if (original[i - 1] == desired[j - 1]) {
        search(fout, original, desired, dp, op, index, result, minSize, i - 1, j - 1, found);
        return;
    }
    
    if (dp[i][j] == dp[i - 1][j - 1] + 1) {
        op.push_back("Replace");
        index.push_back(j - 1);
        search(fout, original, desired, dp, op, index, result, minSize, i - 1, j - 1, found);
        op.pop_back();
        index.pop_back();
    }
    
    if (dp[i][j] == dp[i][j - 1] + 1) {
        op.push_back("Insert");
        index.push_back(j - 1);
        search(fout, original, desired, dp, op, index, result, minSize, i, j - 1, found);
        op.pop_back();
        index.pop_back();
    }
    
    if (dp[i][j] == dp[i - 1][j] + 1) {
        op.push_back("Delete");
        index.push_back(i - 1);
        search(fout, original, desired, dp, op, index, result, minSize, i - 1, j, found);
        op.pop_back();
        index.pop_back();
    }
    

}

int main(int argc, char **argv)
{
    string fileName = argv[1];
	ifstream fin(fileName);
    string outputFstr = fileName.substr(0, fileName.size() - 3);
    outputFstr += ".out";
    ofstream fout(outputFstr);
    
    int numOriginal, numDesired;
    vector<string> original, desired;
    
    string str;
    fin >> str;  // Read Command
    fin >> numOriginal;
    fin.ignore(1000, '\n');
    for (int i = 0; i < numOriginal; ++i) {
        getline(fin, str);
        original.push_back(str);
    }
    getline(fin, str);
    
    fin >> str;  // Read Command
    fin >> numDesired;
    fin.ignore(1000, '\n');
    for (int i = 0; i < numDesired; ++i) {
        getline(fin, str);
        desired.push_back(str);
    }
    
    vector<int> oneD(numDesired + 1, 0);
    vector<vector<int> > dp(numOriginal + 1, oneD);
    for (int i = 0; i < numOriginal; ++i) {
        dp[i][0] = i;
    }
    for (int j = 0; j < numDesired + 1; ++j) {
        dp[0][j] = j;
    }
    
    for (int i = 1; i <= numOriginal; ++i) {
        for (int j = 1; j <= numDesired; ++j) {
            if (original[i - 1] == desired[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = min(min(dp[i - 1][j - 1], dp[i - 1][j]), dp[i][j - 1]) + 1;
            }
        }
    }
    bool found = false;
    vector<string> op;
    vector<int> index;
    vector<string> result;
    int minSize = INT_MAX;
    
    search(fout, original, desired, dp, op, index, result, minSize, numOriginal, numDesired, found);

#if 0
    fout << endl;
    fout << dp[numOriginal][numDesired] << endl;
    fout << minSize << endl;
    fout << endl;
#endif

    for (const auto &s: result) {
        fout << s << endl;
    }
    
    fin.close();
    fout.close();
    
	return 0;
}
