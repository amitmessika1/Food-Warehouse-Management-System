#include "Customer.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders)
    : id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId{} {}
    

const string &Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

int Customer::getCustomerDistance() const {
    return locationDistance;
}

int Customer::getMaxOrders() const {
    return maxOrders;
}

int Customer::getNumOrders() const {
    return ordersId.size();
}

bool Customer::canMakeOrder() const {
    if (getNumOrders() < maxOrders) 
        return true;
    return false;
}

const vector<int> &Customer::getOrdersIds() const {
    return ordersId;
}

int Customer::addOrder(int orderId) {
    if (canMakeOrder()){  //cheak if he can make order
        ordersId.push_back(orderId);
        return orderId;
    }
    else return -1;
}

//copy constructor
Customer::Customer(const Customer &other)
    : id(other.id), name(other.name), locationDistance(other.locationDistance), maxOrders(other.maxOrders), ordersId(other.ordersId) {}

// Implementation for SoldierCustomer methods

SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders)
    : Customer(id, name, locationDistance, maxOrders) {}

SoldierCustomer* SoldierCustomer::clone() const {
    return new SoldierCustomer(*this);
}

//copy constructor
SoldierCustomer::SoldierCustomer(const SoldierCustomer &other): Customer(other) {}

// Implementation for CivilianCustomer methods

CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders)
    : Customer(id, name, locationDistance, maxOrders) {}

CivilianCustomer* CivilianCustomer::clone() const {
    return new CivilianCustomer(*this);
}

//copy constructor
CivilianCustomer::CivilianCustomer(const CivilianCustomer &other): Customer(other) {}