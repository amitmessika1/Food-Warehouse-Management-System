#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;


class Order;


class Customer {
    public:
        Customer(int id, const string &name, int locationDistance, int maxOrders);
        const string &getName() const;
        int getId() const;
        int getCustomerDistance() const;
        int getMaxOrders() const; //Returns maxOrders
        int getNumOrders() const; //Returns num of orders the customer has made so far
        bool canMakeOrder() const; //Returns true if the customer didn't reach max orders
        const vector<int> &getOrdersIds() const;
        int addOrder(int orderId); //return OrderId if order was added successfully, -1 otherwise
        virtual Customer *clone() const = 0; // Return a copy of the customer
        

        Customer(const Customer &other); //copy constructor
        virtual ~Customer() = default;
        
    private:
        const int id;
        const string name;
        const int locationDistance;
        const int maxOrders;
        vector<int> ordersId;
};


class SoldierCustomer: public Customer {
    public:
        SoldierCustomer(int id, string name, int locationDistance, int maxOrders);
        SoldierCustomer *clone() const override;
        SoldierCustomer(const SoldierCustomer &other); //copy constructor
    private:
        
};

class CivilianCustomer: public Customer {
    public:
        CivilianCustomer(int id, string name, int locationDistance, int maxOrders);
        CivilianCustomer *clone() const override;
        CivilianCustomer(const CivilianCustomer &other); //copy constructor
    private:
        
};