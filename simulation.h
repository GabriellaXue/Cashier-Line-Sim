/**
 * @file simulation.h
 * Class to handle cashier simulation.
 */

#pragma once
#include <list>
using std::list;

/** Id for uniquely identify customer */
static unsigned long uid = 0;

/**
 * TODO: class definition.
 */
class Simulation {
    private:
        struct Customer {
            unsigned long id; // unique id for customer
            string type; // customer type
            int itemNum; // number of items the customer is about to checkout
            int arrivalT; // timestamp the customer arrives
            int currReg; // which register did the customer go to
            int waitNum; // how many people in front of the customer
            Customer(unsigned long id, string type, int itemNum, int arrivalT)
                : id(id), type(type), itemNum(itemNum), arrivalT(arrivalT) {}
        };

        /**
         * Number of registers in total.
         */
        int numRegister_;

        /**
         * The list of customers in tuple ({customer type}, {time arrive}, {number of items}).
         */
        queue< tuple<string, int, int> > customerList_;

        /**
         * Track of the number of customers in each lines.
         */
        vector<int> regQueue_;

        /**
         * Generate unique id for each customer.
         * @return A customer id.
         */
        unsigned long generateId();

        /**
         * Output simulation info.
         */
        void printInfo();

        /**
         * Customers waiting to be served at the same time.
         * @return a list of customers.
         */
        vector<Customer> serveNext();

        /**
         * A utility function to swap two values.
         * @param a first value.
         * @param b second value.
         */
        void swap(Customer& a, Customer& b);

        /**
         * Sort customer list based on item number.
         * @param toSort a list of customer to be sorted.
         */
        void sortItemNum(vector<Customer> &toSort, int start, int end);

        /**
         * Helper function for sorting.
         * @param toSort a list of value to be partitioned.
         * @param start the begin idx of the list.
         * @param end the end idx of the list.
         * @param pivotIdx the pivot idx of the list.
         * @return the pivotIdx after sorting
         */
        int partition(vector<Customer> &toSort, int start, int end, int pivotIdx);

        /**
         * TODO: ?
         */
        void processService(list<Customer> tmpList);

    public:
        /**
         * Constructs a Simulation with registers number and customer list.
         * @param numReg number of registers.
         * @param customerLs list of customer with format ({customer type}, {time arrive}, {number of items}).
         */
        Simulation(int numReg, queue< tuple<string, int, int> > customerLs);

        /**
         * @return timestamp of the simulation.
         */
        int simulate();

};

#include "simulation.cpp"
