#include <iostream>
#include <fstream>
#include <sstream>
#include "WareHouse.h"
#include "Parse.h"
#include "Action.h"

WareHouse::WareHouse(const string &configFilePath) : isOpen(false), actionsLog{}, volunteers{}, pendingOrders{}, inProcessOrders{}, completedOrders{}, customers{},
customerCounter(0) ,volunteerCounter(0), orderCounter(0), errorCustomer(new SoldierCustomer(-1, "errorCustomer" , -1, -1)), 
errorVolunteer(new CollectorVolunteer(-1, "errorVolunteer", -1)), errorOrder(new Order(0,0,0)){

    Parse myParse(configFilePath);
    int numOfLines = myParse.getNumOfLines();
    for (int i=1; i<= numOfLines; i++){   //decipher each line in the config file
        string line = myParse.getLine(i);
        size_t spacePos = line.find(' '); //counting the num of spaces to recognize each word
        std::string firstWord = line.substr(0, spacePos);
        if (firstWord == "customer") {
            size_t nextSpacePos = line.find(' ', spacePos + 1); //promote to the next word
            string customerName = line.substr(spacePos + 1, nextSpacePos - spacePos - 1);

            spacePos = nextSpacePos;
            nextSpacePos = line.find(' ', spacePos + 1); //promote to the next word
            string customerType = line.substr(spacePos + 1, nextSpacePos - spacePos - 1);

            spacePos = nextSpacePos;
            nextSpacePos = line.find(' ', spacePos + 1); //promote to the next word
            int customerDis = std::stoi(line.substr(spacePos + 1, nextSpacePos - spacePos - 1));

            spacePos = nextSpacePos;
            int maxOrder = std::stoi(line.substr(spacePos + 1)); 

            if (customerType == "soldier") 
               customers.push_back(new SoldierCustomer(customerCounter, customerName, customerDis, maxOrder));
            else 
                customers.push_back(new CivilianCustomer(customerCounter, customerName, customerDis, maxOrder));
            customerCounter++;
        }

        else if (firstWord == "volunteer"){
            size_t nextSpacePos = line.find(' ', spacePos + 1); //promote to the next word
            string volunteerName = line.substr(spacePos + 1, nextSpacePos - spacePos - 1);

            spacePos = nextSpacePos;
            nextSpacePos = line.find(' ', spacePos + 1); //promote to the next word
            string voulnteerType = line.substr(spacePos + 1, nextSpacePos - spacePos - 1);

            spacePos = nextSpacePos;
            nextSpacePos = line.find(' ', spacePos + 1); //promote to the next word
            int time = std::stoi(line.substr(spacePos + 1, nextSpacePos - spacePos - 1));

            if (voulnteerType == "collector")
                volunteers.push_back(new CollectorVolunteer(volunteerCounter, volunteerName, time)); 

            else if (voulnteerType == "limited_collector") { 
                spacePos = nextSpacePos;
                int maxOrder = std::stoi(line.substr(spacePos + 1));
                volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter, volunteerName, time, maxOrder));
            }
             else if (voulnteerType == "driver"){
                spacePos = nextSpacePos;
                int distancePerStep = std::stoi(line.substr(spacePos + 1));
                volunteers.push_back(new DriverVolunteer(volunteerCounter, volunteerName, time, distancePerStep)); 
             }
             else{ //limited_driver
                spacePos = nextSpacePos;
                int distancePerStep = std::stoi(line.substr(nextSpacePos + 1, line.find(' ', nextSpacePos + 1) - nextSpacePos - 1));

                spacePos = nextSpacePos;
                nextSpacePos = line.find(' ', spacePos + 1);
                int maxOrder = std::stoi(line.substr(nextSpacePos + 1));
                volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter, volunteerName, time, distancePerStep, maxOrder));
             }
             volunteerCounter++;
        }

    }

}

void WareHouse::start() {
    open();
    isOpen = true;

    while (isOpen){
        std::string userInput;
        std::getline(std::cin, userInput); 

        std::istringstream iss(userInput);
        std::string actionType;
        iss >> actionType; // get the first word in the linei

        if(actionType == "step"){
            int numOfSteps;
            iss >> numOfSteps; // get the second word in the line
            SimulateStep* action = new SimulateStep(numOfSteps);
            action->act(*this);
        }

        else if(actionType == "order"){
            int customerId;
            iss >> customerId; // get the second word in the line
            AddOrder* action = new AddOrder(customerId);
            action->act(*this);
        }

        else if(actionType == "customer"){
            string customerName;
            string customerType;
            int distance; // get the second word in the line
            int maxOrders; // get the third word in the line
            iss >> customerName >> customerType >> distance >> maxOrders;
            AddCustomer* action = new AddCustomer(customerName,customerType, distance, maxOrders);
            action->act(*this);
        }

        else if(actionType == "orderStatus"){
            int orderId;
            iss >> orderId; // get the second word in the line
            PrintOrderStatus* action = new PrintOrderStatus(orderId);
            action->act(*this);
        }

        else if(actionType == "customerStatus"){
            int customerId;
            iss >> customerId; // get the second word in the line
            PrintCustomerStatus* action = new PrintCustomerStatus(customerId);
            action->act(*this);
        }

        else if(actionType == "volunteerStatus"){
            int volunteerId;
            iss >> volunteerId; // get the second word in the line
            PrintVolunteerStatus* action = new PrintVolunteerStatus(volunteerId);
            action->act(*this);
        }

        else if(actionType == "log"){
            PrintActionsLog* action = new PrintActionsLog();
            action->act(*this);
        }

        else if(actionType == "close"){
            Close* action = new Close();
            action->act(*this);
        }

        else if(actionType == "backup"){
            BackupWareHouse* action = new BackupWareHouse();
            action->act(*this);
        }

        else if(actionType == "restore"){
            RestoreWareHouse* action = new RestoreWareHouse();
            action->act(*this);
        }

        else{  
            cout << "invalid action type" << endl;
        }
    }
}

void WareHouse::addOrder(Order* order) {
    pendingOrders.push_back(order);   
    (getCustomer(order->getCustomerId())).addOrder(order->getId());
    orderCounter++;      
}

void WareHouse::addAction(BaseAction* action) {
    actionsLog.push_back(action);
}

Customer& WareHouse::getCustomer(int customerId) const {
    if (customerId < customerCounter && customerId >= 0){  //check if the id is exist
        for (size_t i=0 ; i< customers.size() ; i++){
            if (customers[i]->getId() == customerId)
                return *customers[i];
        }
    }

    return *errorCustomer;     
}

Volunteer& WareHouse::getVolunteer(int volunteerId) const {
    if (volunteerId < volunteerCounter && volunteerId >= 0){ //check if the id is exist
        for (size_t i=0 ; i< volunteers.size() ; i++){
        if (volunteers[i]->getId() == volunteerId)
            return *volunteers[i];
        }  
    }

    return *errorVolunteer;
}

Order& WareHouse::getOrder(int orderId) const {
    for (size_t i=0 ; i< pendingOrders.size() ; i++){  //search the specific order in the pendingOrders Vector
        if (pendingOrders[i]->getId() == orderId)
            return *pendingOrders[i];
    } 
    for (size_t i=0 ; i< inProcessOrders.size() ; i++){ //search the specific order in the inProcessOrders Vector
        if (inProcessOrders[i]->getId() == orderId)
            return *inProcessOrders[i];
    }
    for (size_t i=0 ; i< completedOrders.size() ; i++){ //search the specific order in the completedOrders Vector
        if (completedOrders[i]->getId() == orderId)
            return *completedOrders[i];
    }

    return *errorOrder;
}

const vector<BaseAction*>& WareHouse::getActions() const {
    return actionsLog;
}

void WareHouse::close() {
   isOpen = false;

   for (size_t i=0 ; i< pendingOrders.size() ; i++){
        cout << "OrderID: " + std :: to_string(pendingOrders[i]->getId()) + ",CusromerID: " + std :: to_string(pendingOrders[i]->getCustomerId())
         + ",OrderStatus: " + pendingOrders[i]->toString() << endl;       
    } 
    for (size_t i=0 ; i< inProcessOrders.size() ; i++){
        cout << "OrderID: " + std :: to_string(inProcessOrders[i]->getId()) + ",CusromerID: " + std :: to_string(inProcessOrders[i]->getCustomerId())
         + ",OrderStatus: " + inProcessOrders[i]->toString() << endl;
    }
    for (size_t i=0 ; i< completedOrders.size() ; i++){
        cout << "OrderID: " + std :: to_string(completedOrders[i]->getId()) + ",CusromerID: " + std :: to_string(completedOrders[i]->getCustomerId())
         + ",OrderStatus: " + completedOrders[i]->toString() << endl;
    }
}

void WareHouse::open() {
    cout << "Warehouse is open!" << endl;
}

int WareHouse :: getOrderCounter() const{
    return orderCounter;
}

vector<Order*>& WareHouse::getPendingOrders() {
    return pendingOrders;
}

vector<Order*>& WareHouse::getInProcessOrders() {
    return inProcessOrders;
}

vector<Order*>& WareHouse::getCompletedOrders() {
    return completedOrders;
}

vector<Volunteer*>& WareHouse::getVectorVolunteers() {
    return volunteers;
}


int WareHouse :: getCustomerCounter() const{
    return customerCounter;
}

void WareHouse::addCustomer(Customer* customer) {
    customers.push_back(customer);   
    customerCounter++;      
}

int WareHouse :: getVolunteerCounter() const{
    return volunteerCounter;
}


// destructor
WareHouse:: ~WareHouse(){

    for(BaseAction* action: actionsLog){                        
        delete action;
    }
    actionsLog.clear();                                

    for(Volunteer* volunteer: volunteers){                        
        delete volunteer;
    }
    volunteers.clear();

    for ( Order* order : pendingOrders ){
        delete order;
    }
    pendingOrders.clear();

    for (Order* order : inProcessOrders){
        delete order;
    }
    inProcessOrders.clear();

    for ( Order* order : completedOrders){
        delete order;
    }
    completedOrders.clear();

    for( Customer* customer : customers){
        delete customer;
    }
    customers.clear();

    delete errorCustomer;
    delete errorVolunteer;
    delete errorOrder;
}

//copy constructor
WareHouse :: WareHouse(const WareHouse &other): 
    isOpen(other.isOpen), actionsLog(), volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),customers(),
    customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter),
    errorCustomer(new SoldierCustomer(*(other.errorCustomer))), errorVolunteer(new CollectorVolunteer(*(other.errorVolunteer))), 
    errorOrder(new Order(*(other.errorOrder))){

    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }

    for (Volunteer* volunteer : other.volunteers) {
        volunteers.push_back(volunteer->clone());
    }

    for (Order* order : other.pendingOrders) {
        pendingOrders.push_back(new Order(*order));
    }

    for (Order* order : other.inProcessOrders) {
        inProcessOrders.push_back(new Order(*order));
    }

    for (Order* order : other.completedOrders) {
        completedOrders.push_back(new Order(*order));
    }

    for (Customer* customer : other.customers) {
        customers.push_back(customer->clone());
    }

} 

//Copy assignment operator
WareHouse& WareHouse:: operator=(const WareHouse &other){

    if(this != &other){

        for(BaseAction* action: actionsLog){                        
            delete action;
        }
        actionsLog.clear();
                                   
        for (const BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }

        for(Volunteer* volunteer: volunteers){                        
            delete volunteer;
        }
        volunteers.clear();

        for (const Volunteer* volunteer : other.volunteers) {
            volunteers.push_back(volunteer->clone());
        }

        for ( Order* order : pendingOrders ){
            delete order;
        }
        pendingOrders.clear();

        for (const Order* order : other.pendingOrders) {
            pendingOrders.push_back(new Order(*order));
        }

        for (Order* order : inProcessOrders){
            delete order;
        }
        inProcessOrders.clear();

        for (const Order* order : other.inProcessOrders) {
            inProcessOrders.push_back(new Order(*order));
        }

        for ( Order* order : completedOrders){
            delete order;
        }
        completedOrders.clear();

        for (const Order* order : other.completedOrders) {
            completedOrders.push_back(new Order(*order));
        }

        for( Customer* customer : customers){
            delete customer;
        }
        customers.clear();

        for (const Customer* customer : other.customers) {
            customers.push_back(customer->clone());
        }

        this-> isOpen = other.isOpen;
        this->customerCounter = other.customerCounter;
        this->volunteerCounter = other.customerCounter;
        this->orderCounter = other.orderCounter; 
        delete this->errorCustomer;
        this->errorCustomer = new SoldierCustomer(*(other.errorCustomer));
        delete this->errorVolunteer;
        this->errorVolunteer = new CollectorVolunteer(*(other.errorVolunteer));
        delete errorOrder;
        this->errorOrder = new Order(*(other.errorOrder));
    }

    return *this;
}

//move constructor
WareHouse :: WareHouse (WareHouse &&other) : isOpen(other.isOpen), actionsLog(std:: move(other.actionsLog)), volunteers(std:: move(other.volunteers)),
    pendingOrders(std:: move(other.pendingOrders)),inProcessOrders(std:: move(other.inProcessOrders)),completedOrders(std:: move(other.completedOrders)),
    customers(std:: move(other.customers)),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),
    orderCounter(other.orderCounter),errorCustomer(other.errorCustomer), errorVolunteer(other.errorVolunteer), errorOrder(other.errorOrder){

    int i=0;
        while ((other.actionsLog).size() > 0){
           (other.actionsLog[i]) = nullptr;
           i++;
        }
        actionsLog.clear();
        
        i=0; 
        while ((other.volunteers).size() > 0){
            (other.volunteers[i]) = nullptr;
            i++;
        }
        volunteers.clear();

        i=0; 
        while ((other.pendingOrders).size() > 0){
            (other.pendingOrders[i]) = nullptr;
            i++;
        }
        pendingOrders.clear();

        i=0; 
        while ((other.inProcessOrders).size() > 0){
            (other.inProcessOrders[i])= nullptr;
            i++;
        }
        inProcessOrders.clear();

        i=0; 
        while ((other.completedOrders).size() > 0){
            (other.completedOrders[i]) = nullptr;
            i++;
        }
        completedOrders.clear();

        i=0; 
        while ((other.customers).size() > 0){
            (other.customers[i]) = nullptr;
            i++;
        }
        customers.clear();
    

    other.errorCustomer = nullptr;
    other.errorVolunteer = nullptr;
    other.errorOrder = nullptr;
    other.isOpen = false;
    other.customerCounter = 0;
    other.volunteerCounter = 0;
    other.orderCounter = 0;
}


//move assignment operator
const WareHouse& WareHouse :: operator=(WareHouse &&other){

    if(this != &other){
        for(BaseAction* action: actionsLog){                        
            delete action;
        }
        actionsLog.clear();                                

        for(Volunteer* volunteer: volunteers){                        
            delete volunteer;
        }
        volunteers.clear();

        for ( Order* order : pendingOrders ){
            delete order;
        }
        pendingOrders.clear();

        for (Order* order : inProcessOrders){
            delete order;
        }
        inProcessOrders.clear();

        for ( Order* order : completedOrders){
            delete order;
        }
        completedOrders.clear();

        for( Customer* customer : customers){
            delete customer;
        }
        customers.clear();

        actionsLog = std::move(other.actionsLog);
        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);
        customers = std::move(other.customers);

        int i=0;
        while ((other.actionsLog).size() > 0){
           (other.actionsLog[i]) = nullptr;
           i++;
        }
        actionsLog.clear();
        
        i=0; 
        while ((other.volunteers).size() > 0){
            (other.volunteers[i]) = nullptr;
            i++;
        }
        volunteers.clear();

        i=0; 
        while ((other.pendingOrders).size() > 0){
            (other.pendingOrders[i]) = nullptr;
            i++;
        }
        pendingOrders.clear();

        i=0; 
        while ((other.inProcessOrders).size() > 0){
            (other.inProcessOrders[i])= nullptr;
            i++;
        }
        inProcessOrders.clear();

        i=0; 
        while ((other.completedOrders).size() > 0){
            (other.completedOrders[i]) = nullptr;
            i++;
        }
        completedOrders.clear();

        i=0; 
        while ((other.customers).size() > 0){
            (other.customers[i]) = nullptr;
            i++;
        }
        customers.clear();

        this-> isOpen = other.isOpen;
        other.isOpen = false;
        this->customerCounter = other.customerCounter;
        other.customerCounter = 0;
        this->volunteerCounter = other.customerCounter;
        other.volunteerCounter = 0;
        this->orderCounter = other.orderCounter; 
        other.orderCounter = 0;
        this->errorCustomer = other.errorCustomer;
        other.errorCustomer = nullptr;
        this->errorVolunteer = other.errorVolunteer;
        other.errorVolunteer = nullptr;
        this->errorOrder = other.errorOrder;
        other.errorOrder = nullptr;
    }

    return *this;

}

