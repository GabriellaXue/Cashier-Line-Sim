/**
 * @file simulation.cpp
 * Implementation of Simulation class which is used to handle cashier simulation.
 */

#include <vector>
using std::vector;

#include <tuple>
using std::tuple;

#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::endl;

#include <queue>
using std::queue;

#include <list>
using std::list;

#include <algorithm>
using std::make_heap;
using std::reverse;
using std::sort_heap;

#include <utility>
using std::pair;
using std::make_pair;

#include <sstream>

#include <map>
using std::map;

/**
 * Constructs a Simulation with registers number and customer list.
 * Intialize register queue as empty for all the registers.
 * NOTE: register number is one indexed for register queue.
 * @param numReg number of registers
 * @param customerLs list of customer with format ({customer type}, {time arrive}, {number of items})
 */
Simulation::Simulation(int numReg, queue< tuple<string, int, int> > customerLs) {
    numRegister_ = numReg;
    customerList_ = customerLs;
    for (int i = 0; i < numRegister_; i++) {
        regQueue_.push_back(make_pair(0, std::to_string(i+1)));
        queue<Customer> newQueue;
        regMap_[i + 1] = newQueue;
    }
    make_heap(regQueue_.begin(), regQueue_.end());
    reverse(regQueue_.begin(), regQueue_.end());
}

/**
 * Time is measured in minutes.
 * 
 * The training cashier is assign to the highest number register.
 * 
 * Regular cashier takes one minute to process each item,
 * while training cashier takes two.
 * 
 * Simulation starts at t = 0, while all registers are empty.
 * 
 * Customer type:
 *  A: choose the shortest line
 *  B: choose empty line, or check last customer item number, the less the better
 * 
 * Customers finish checking out are not in line.
 * 
 * If customers arrive at the same time, who has less items choose line first.
 * It same number of items, A choose first.
 */
int Simulation::simulate() {
    while (!customerList_.empty()) {
        vector<Simulation::Customer> waitToServe = serveNext();
        sortItemNum(waitToServe, 0, waitToServe.size() - 1);
        list<Customer> tmpList;
        int count = 1;
        for (auto customer : waitToServe) {
            if (customer.itemNum == count) {
                if (customer.type.compare("A") == 0) {
                    tmpList.push_front(customer);
                } else {
                    tmpList.push_back(customer);
                }
            } else {
                processService(tmpList);
                tmpList.clear();
                count ++;
                if (customer.type.compare("A") == 0) {
                    tmpList.push_front(customer);
                } else {
                    tmpList.push_back(customer);
                }
            }
        }
        processService(tmpList);
        tmpList.clear();
    }
    return checkTime();
}

/**
 * Check the time in each register line and output the longest one.
 * @return return the longest time.
 */
int Simulation::checkTime() {
    int longestTime = 0;
    for (auto it = regMap_.begin(); it != regMap_.end(); it ++) {
        queue<Customer> toCheck = it->second;
        Simulation::Customer currCustomer = toCheck.front();
        int begin = currCustomer.arrivalT;
        begin += currCustomer.processingT;
        toCheck.pop();
        while(!toCheck.empty()) {
            currCustomer = toCheck.front();
            if (begin >= currCustomer.arrivalT) {
            } else {
                begin = currCustomer.arrivalT;
            }
            begin += currCustomer.processingT;
            toCheck.pop();
        }
        longestTime = (longestTime < begin) ? begin : longestTime;
    }
    printInfo(longestTime);
    return longestTime;
}

/**
 * Enqueue customer according to the type requirement.
 * @param tmpList current customer list within the same timestamp
 */
void Simulation::processService(list<Simulation::Customer> tmpList) {
    for (auto customer : tmpList) {
        
        // check if could pop
        for (auto it = regMap_.begin(); it != regMap_.end(); it ++) {
            queue<Customer> toCheck = it->second;
            if (toCheck.empty()) {
                continue;
            }
            Simulation::Customer currCustomer = toCheck.front();
            if (currCustomer.arrivalT + currCustomer.processingT <= customer.arrivalT) {
                toCheck.pop();
                // update the `regQueue_` as well
                for (int i = 0; i < regQueue_.size(); i++) {
                    if (regQueue_[i].second.compare(std::to_string(it->first)) == 0) {
                        regQueue_[i].first --;
                    }
                }
            }
        }

        pair<int, string> potentialPos = regQueue_.front();
        stringstream str(potentialPos.second);
        int toInt = 0;
        str >> toInt;
        pair<int, string> bestPos;

        // avoid using training register if there are other available ones
        bool found = false;
        if (toInt == numRegister_) {
            int tmp = potentialPos.first;
            for (auto item : regQueue_) {
                if (item.first == tmp && item.second.compare(potentialPos.second)) {
                    bestPos = item;
                    found = true;
                    break;
                }
            }
            if (!found) {
                bestPos = potentialPos;
            }
        } else {
            bestPos = potentialPos;
        }
        stringstream tmpStr(bestPos.second);
        int tmpInt = 0;
        tmpStr >> tmpInt;
        if (customer.type.compare("A") == 0) {
            customer.processingT = (tmpInt == numRegister_) ? customer.itemNum * 2 : customer.itemNum;
            customer.currReg = bestPos.second;
            customer.waitNum = bestPos.first;
            regMap_[tmpInt].push(customer);
            for (int i = 0; i < regQueue_.size(); i++) {
                if (regQueue_[i].second.compare(bestPos.second) == 0) {
                    regQueue_[i].first ++;
                }
            }
            sort_heap(regQueue_.begin(), regQueue_.end());
        } else {
            if (bestPos.first == 0) {
                customer.processingT = (tmpInt == numRegister_) ? customer.itemNum * 2 : customer.itemNum;
                customer.currReg = bestPos.second;
                customer.waitNum = 0;
                regMap_[tmpInt].push(customer);
                for (int i = 0; i < regQueue_.size(); i++) {
                if (regQueue_[i].second.compare(bestPos.second) == 0) {
                    regQueue_[i].first ++;
                }
            }
                sort_heap(regQueue_.begin(), regQueue_.end());
            } else {
                int minItem = 0;
                int bestLine = numRegister_;
                for (auto it = regMap_.begin(); it != regMap_.end(); it++) {
                    Simulation::Customer lastCustomer = it->second.back();
                    if (lastCustomer.itemNum == minItem) {
                        if (it->first < bestLine) {
                            bestLine = it->first;
                        }
                    } else if (lastCustomer.itemNum == minItem) {
                        minItem = lastCustomer.itemNum;
                        bestLine = it->first;
                    }
                }
                customer.processingT = (bestLine == numRegister_) ? customer.itemNum * 2 : customer.itemNum;
                customer.currReg = bestLine;
                customer.waitNum = regMap_[bestLine].size();
                regMap_[bestLine].push(customer);
                for (int i = 0; i < regQueue_.size(); i++) {
                    if (regQueue_[i].second.compare(std::to_string(bestLine)) == 0) {
                        regQueue_[i].first ++;
                        sort_heap(regQueue_.begin(), regQueue_.end());
                    }
                }
            }   
        }
    }
}

/**
 * Sort customer list based on item number.
 * @param toSort a list of customer to be sorted.
 */
void Simulation::sortItemNum(vector<Simulation::Customer> &toSort, int start, int end) {
    if (start >= end) return;
    int mid = (end + start) / 2;
    int pivot = partition(toSort, start, end, mid);
    sortItemNum(toSort, start, pivot - 1);
    sortItemNum(toSort, pivot + 1, end);
}

/**
 * A utility function to swap two values.
 */
void Simulation::swap(Simulation::Customer& a, Simulation::Customer& b) {
    unsigned long tmpId = a.id;
    string tmpType = a.type;
    int tmpItemNum = a.itemNum;
    int tmpTime = a.arrivalT;
    a.id = b.id;
    a.type = b.type;
    a.itemNum = b.itemNum;
    a.arrivalT = b.arrivalT;
    b.id = tmpId;
    b.type = tmpType;
    b.itemNum = tmpItemNum;
    b.arrivalT = tmpTime;
}

/**
 * This function takes the middle element as pivot, 
 * places the pivot element at its correct position in sorted array,
 * and places all smaller (smaller than pivot) to left of pivot
 * and all greater elements to right of pivot.
 * 
 * @param toSort a list of value to be partitioned.
 * @param start the begin idx of the list.
 * @param end the end idx of the list.
 * @param pivotIdx the pivot idx of the list.
 * @return the pivotIdx after sorting
 */
int Simulation::partition(vector<Simulation::Customer> &toSort, int start, int end, int pivotIdx) {
    int pivotVal = toSort[pivotIdx].itemNum;
    Simulation::swap(toSort[pivotIdx], toSort[end]);
    int storeIdx = start;
    for (int i = start; i < end; i++) {
        if (toSort[i].itemNum > pivotVal) {
            Simulation::swap(toSort[i], toSort[storeIdx]);
            storeIdx++;
        }
    }
    Simulation::swap(toSort[storeIdx], toSort[end]);
    return storeIdx;
}


/**
 * Customers waiting to be served at the same time.
 * @return a list of current waiting customers.
 */
vector<Simulation::Customer> Simulation::serveNext() {
    int currTime = 0;
    vector<Simulation::Customer> sameTimeArrival;
    while (!customerList_.empty()) {
        tuple<string, int, int> currCustomer = customerList_.front();
        int customerTime = std::get<1>(currCustomer);
        if (currTime == 0) {
            currTime = customerTime;
        } else if (customerTime != currTime) {
            break;
        }
        unsigned long uid = generateId();
        string customerType = std::get<0>(currCustomer);
        int itemNumber = std::get<2>(currCustomer);
        Simulation::Customer newCustomer(uid, customerType, itemNumber, customerTime);
        sameTimeArrival.push_back(newCustomer);
        customerList_.pop();
    }
    return sameTimeArrival;
}

/**
 * Output simulation info.
 * @param num a time integer.
 */
void Simulation::printInfo(int num) {
    std::cout << "Finished at: t=" << num << " " << "minutes" << std::endl;
}

/**
 * Generate unique id for each customer.
 * @return A customer id.
 */
unsigned long Simulation::generateId() {
    return ++uid;
}
