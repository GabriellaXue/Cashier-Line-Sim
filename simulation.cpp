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

/**
 * Constructs a Simulation with registers number and customer list.
 * Intialize register queue as empty for all the registers.
 * NOTE: register number is zero indexed for register queue.
 * @param numReg number of registers
 * @param customerLs list of customer with format ({customer type}, {time arrive}, {number of items})
 */
Simulation::Simulation(int numReg, queue< tuple<string, int, int> > customerLs) {
    numRegister_ = numReg;
    customerList_ = customerLs;
    for (int i = 0; i < numRegister_; i++) {
        regQueue_.push_back(0);
    }
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
    return 1;
}

void Simulation::processService(list<Simulation::Customer> tmpList) {
    for (auto item : tmpList) {
        cout << item.type << " " << item.arrivalT << " " << item.itemNum << endl;
    }
    cout << "ready for the next round" << endl;
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
        if (toSort[i].itemNum < pivotVal) {
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
 */
void Simulation::printInfo() {
    std::cout << "hello world" << std::endl;
}

/**
 * Generate unique id for each customer.
 * @return A customer id.
 */
unsigned long Simulation::generateId() {
    return ++uid;
}