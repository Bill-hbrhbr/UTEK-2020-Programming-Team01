#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

struct Command{
    int action = 0; //0 for replace, 1 for insert, 2 for delete;
    int index = 0;
    char object;
};

void part2(vector<char>& original, vector<char>& desired, ofstream& fout);
void dynamicPrograming(vector<char>& original, vector<char>& desired, vector<vector<int>>& dp);
void createCommandInform(vector<vector<int>>& dp, vector<Command>& cmd,
                         vector<char>& original, vector<char>& desired);
void printCmd(vector<Command>& cmd, ofstream& fout);

int main(int argc, char **argv)
{
    ifstream fin(argv[1]);
    string outName(argv[1]);
    outName.erase(outName.end() - 3, outName.end());
    outName = outName + ".out";
    ofstream fout(outName);

    string str;

    vector<char> original, desired;

    fin >> str;
    fin.ignore(1000, '\n');
    for(char & c : str) original.push_back(c);

    fin >> str;
    for(char & c : str) desired.push_back(c);

    part2(original, desired, fout);


    fin.close();
    fout.close();
    return 0;
}

void part2(vector<char>& original, vector<char>& desired, ofstream& fout) {
    vector<int> leo(desired.size() + 1, 0);
    vector<vector<int>> dp(original.size() + 1, leo);
    dynamicPrograming(original, desired, dp);
    vector<Command> cmd;
    createCommandInform(dp, cmd, original, desired);
    printCmd(cmd, fout);
}

void dynamicPrograming(vector<char>& original, vector<char>& desired, vector<vector<int>>& dp) {
    int oSize = original.size() + 1;
    int dSize = desired.size() + 1;
    for(int i = 0; i < oSize; i ++) dp[i][0] = i;
    for(int j = 0; j < dSize; j ++) dp[0][j] = j;

    for(int i = 1; i < oSize; i ++) {
        for(int j = 1; j < dSize; j ++) {
            if(original[i - 1] == desired[j - 1]) dp[i][j] = dp[i - 1][j - 1];
            else dp[i][j] = min(dp[i - 1][j],  min(dp[i][j - 1], dp[i - 1][j - 1])) + 1;
        }
    }
}

void printCmd(vector<Command>& cmd, ofstream& fout) {
    for(Command& source : cmd) {
        if(source.action == 0) fout << "Replace ";
        else if(source.action == 1) fout << "Insert ";
        else if(source.action == 2) fout << "Delete ";

        fout << source.index;
        if(source.action != 2) fout << ", \'" << source.object << "\'";
        fout << endl;
    }
    fout << endl;
}

void createCommandInform(vector<vector<int>>& dp, vector<Command>& cmd,
                         vector<char>& original, vector<char>& desired) {
    int sizeChange = 0;
    int oSize = dp.size();
    int dSize = dp[0].size();
    stack<Command> informStored;

    int i = oSize - 1;
    int j = dSize - 1;

    while(i >= 0 && j >= 0) {
        Command newCmd;
        if(i == 0 && j == 0) break;
        else if(i == 0) {
            newCmd.action = 1;
            newCmd.index = 0;
            newCmd.object = desired[j - 1];
            informStored.push(newCmd);
            j --;
        } else if(j == 0) {
            newCmd.action = 2;
            newCmd.index = i - 1;
            newCmd.object = original[i - 1];
            informStored.push(newCmd);
            i --;
        } else {
            if(original[i - 1] == desired[j - 1]){
                i --;
                j --;
            } else {
                int minStep = min(dp[i - 1][j],  min(dp[i][j - 1], dp[i - 1][j - 1]));
                if(dp[i - 1][j - 1] == minStep) {
                    newCmd.action = 0;
                    newCmd.index = i - 1;
                    newCmd.object = desired[j - 1];
                    informStored.push(newCmd);
                    i --;
                    j --;
                } else if(dp[i - 1][j] == minStep) {
                    newCmd.action = 2;
                    newCmd.index = i - 1;
                    newCmd.object = original[i - 1];
                    informStored.push(newCmd);
                    i --;
                } else {
                    newCmd.action = 1;
                    newCmd.index = i;
                    newCmd.object = desired[j - 1];
                    informStored.push(newCmd);
                    j --;
                }
            }
        }
    }

    while(!informStored.empty()) {
        informStored.top().index += sizeChange;
        if(informStored.top().action == 1) sizeChange ++;
        else if(informStored.top().action == 2) sizeChange --;
        cmd.push_back(informStored.top());
        informStored.pop();
    }
}







