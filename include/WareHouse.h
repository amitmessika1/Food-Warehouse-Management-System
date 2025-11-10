#pragma once
#include <string>
#include <vector>
using namespace std;

#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        int getOrderCounter() const;
        vector<Order*> &getPendingOrders();
        vector<Order*> &getInProcessOrders();
        vector<Order*> &getCompletedOrders();
        vector<Volunteer*> &getVectorVolunteers();
        int getCustomerCounter() const;
        void addCustomer(Customer* customer);
        int getVolunteerCounter() const;

        //Rule of 5
        ~WareHouse(); // destructor
        WareHouse(const WareHouse &other); //copy constructor
        WareHouse& operator=(const WareHouse &other); //Copy assignment operator
        WareHouse (WareHouse &&other); //move constructor
        const WareHouse& operator=(WareHouse &&other); //move assignment operator


    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter; //For assigning unique order IDs
        SoldierCustomer* errorCustomer;
        CollectorVolunteer* errorVolunteer;
        Order* errorOrder;
};
