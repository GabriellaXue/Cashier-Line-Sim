#include <iostream>
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <tuple>
using std::tuple;

#include <sstream>
using std::stringstream;

#include <queue>
using std::queue;

#include "simulation.h"


int main() {
    const int inputLen = 3;                             // each customer should have three input vars
    const int MAX_LEN = 256;

    int numRegister;                                    // number of registers
    queue< tuple<string, int, int> > customerList;      // all the customers in the input file
    string delimiter = " ";
    char line[MAX_LEN - 1];                             // hold line read from input temporarily

    while(std::cin.getline(line, MAX_LEN)) {
        string tmp(line);
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
            customerList.push(newTuple);
        }
    }

    // FOR TESTING USE
    // cout << get<0>(customerList[0]) << " " << get<1>(customerList[0]) << " " << get<2>(customerList[0]) << " " << endl;
    // cout << get<0>(customerList[1]) << " " << get<1>(customerList[1]) << " " << get<2>(customerList[1]) << " " << endl;
    // cout << numRegister << endl;

    Simulation newSim(numRegister, customerList);
    int holdVal = newSim.simulate();
    return 0;
}