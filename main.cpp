#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <string>
#include <sstream>

using namespace std;

int main() {
    const int inputLen = 3;                             // each customer should have three input vars

    int numRegister;                                    // number of registers
    string tmp;                                         // hold string read from file temporarily
    vector< tuple<string, int, int> > customerList;        // all the customers in the input file
    ifstream infile;
    string delimiter = " ";

    infile.open("input.txt");
    while(!infile.eof()) {
        getline(infile, tmp);
        if (tmp.length() == 1) {
            stringstream convertNum(tmp);
            convertNum >> numRegister;
        } else {
            size_t pos = 0;
            string token;
            vector<string> splittedVal;
            while((pos = tmp.find(delimiter)) != string::npos) {
                token = tmp.substr(0, pos);
                splittedVal.push_back(token);
                tmp.erase(0, pos + delimiter.length());
            }
            splittedVal.push_back(tmp);
            if (splittedVal.size() != inputLen) {
                cout << "please check input format, [{customer type}, {timestamp}, {item number}]" << endl;
                return 1;
            }
            int second;
            int third;
            stringstream convertNum(splittedVal[1]);
            convertNum >> second;
            stringstream convertNum2(splittedVal[2]);
            convertNum2 >> third;
            tuple<string, int, int> newTuple = make_tuple(splittedVal[0], second, third);
            customerList.push_back(newTuple);
        }
    }
    infile.close();

    // FOR TESTING USE
    // cout << get<0>(customerList[0]) << " " << get<1>(customerList[0]) << " " << get<2>(customerList[0]) << " " << endl;
    // cout << get<0>(customerList[1]) << " " << get<1>(customerList[1]) << " " << get<2>(customerList[1]) << " " << endl;
    // cout << numRegister << endl;
    return 0;
}