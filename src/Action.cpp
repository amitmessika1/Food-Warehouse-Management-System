#include "WareHouse.h"
#include "Action.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Volunteer.h"
#include <algorithm>
extern WareHouse* backup;


// Implementation for BaseAction methods

BaseAction::BaseAction() : errorMsg(), status(){}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = ActionStatus :: COMPLETED;
}

void BaseAction::error(string errorMsg) {
    status = ActionStatus ::ERROR;
    this-> errorMsg = errorMsg;
    cout << "Error: " + errorMsg <<endl;
}

string BaseAction::getErrorMsg() const {
    return errorMsg;
}

//copy constructor
BaseAction:: BaseAction(const BaseAction &other) : errorMsg(other.errorMsg), status(other.status) {}


//SimulateStep methods

SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse) {    
    vector<Order*> &pendingOrd = wareHouse.getPendingOrders();
    vector<Volunteer*> &volunteers = wareHouse.getVectorVolunteers();
    vector<Order*> &inProcessOrd = wareHouse.getInProcessOrders();
    vector<Order*> &completedOrd = wareHouse.getCompletedOrders();

    for (int step =0; step<numOfSteps; step++){

    //step1:
        for(auto i=pendingOrd.begin(); i!= pendingOrd.end(); ){ //Go through all orders in pendingOrders
            bool orderProcessed = false;
            if((*i)->toString() == "PENDING"){  //check if the order's status is pending         
                for(size_t j=0; j<volunteers.size(); j++){ //search for collector volunteer that is available
                    if ((volunteers[j]->toString() == "CollectorVolunteer" || volunteers[j]->toString() == "LimitedCollectorVolunteer" ) && 
                        (*volunteers[j]).canTakeOrder(*(*i))) {
                        volunteers[j]->acceptOrder(*(*i)); //associate the order with the volunteer
                        (*(*i)).setCollectorId(volunteers[j]->getId());
                        (*(*i)).setStatus(OrderStatus::COLLECTING);
                        inProcessOrd.push_back((*i)); //update the order to be in process
                        i = pendingOrd.erase(i); 
                        orderProcessed = true;                       
                        break;                       
                    }
                }
                if(!orderProcessed) i++;
            }

            else if ((*i)->toString() == "COLLECTING"){ //check if the order's status is collecting
                    for(size_t j=0; j<volunteers.size(); j++){ //search for ifdriver volunteer that is available
                        if ((volunteers[j]->toString() == "DriverVolunteer" || volunteers[j]->toString() == "LimitedDriverVolunteer") &&
                            (*volunteers[j]).canTakeOrder(*(*i))) {
                            volunteers[j]->acceptOrder(*(*i)); //associate the order with the volunteer
                            (*(*i)).setDriverId(volunteers[j]->getId());
                            (*(*i)).setStatus(OrderStatus::DELIVERING);
                           
                            inProcessOrd.push_back((*i)); //update the order to be in process
                            i = pendingOrd.erase(i);
                            orderProcessed = true;                            
                            break;
                        
                        }
                    }

                if(!orderProcessed) i++;
            }

            else i++;
        }

    //step2:
        for(size_t i=0; i<volunteers.size(); i++){  //perform step in each volunteer
            volunteers[i]->step();                         
        }

    //step3: 
        for(size_t i =0; i<volunteers.size(); i++){
            int orderId = volunteers[i]->getCompletedOrderId();       
            if(!volunteers[i]->isBusy() && orderId!= NO_ORDER){   // find all the volunteers that finish there processing
                Order* order=nullptr;
                for(size_t j=0; j<inProcessOrd.size(); j++){     //find the order with the orderId
                    if(inProcessOrd[j]->getId() == orderId){
                        order = inProcessOrd[j];
                        break;
                    }
                }                 
               
                if (order == nullptr){}  //if the process on the order is finished do nothing
 
                else{

                    if (order->toString()=="COLLECTING" && 
                        (volunteers[i]->toString() == "CollectorVolunteer" || volunteers[i]->toString() == "LimitedCollectorVolunteer")){                    
                        pendingOrd.push_back(order); //prepare the order to find driver volunteer
                        inProcessOrd.erase(std:: remove(inProcessOrd.begin(), inProcessOrd.end(), order), inProcessOrd.end()); 
                    }

                    else if (order->toString()== "DELIVERING" && 
                        (volunteers[i]->toString() == "DriverVolunteer" || volunteers[i]->toString() == "LimitedDriverVolunteer")){  
                        order->setStatus(OrderStatus::COMPLETED);   //update order's status 
                        completedOrd.push_back(order); 
                        inProcessOrd.erase(std:: remove(inProcessOrd.begin(), inProcessOrd.end(), order), inProcessOrd.end());                                   
                   }   
                }                                         
            }                                            
        }     
     
    //step4:

        for(Volunteer* volunteer: volunteers){  //searching limited Volunteer                       
            if(!volunteer->hasOrdersLeft() && !volunteer->isBusy()){ //checking if he reached his maxOrder limit
                volunteers.erase(std::remove(volunteers.begin(), volunteers.end(), volunteer), volunteers.end());
                delete volunteer;
            }                        
        }
                       
        complete(); 
    }
    wareHouse.addAction(this); //push the action to actionsLog
}

string SimulateStep::toString() const {
    if (getStatus() == ActionStatus :: COMPLETED)
        return "simulateStep "+ std:: to_string(numOfSteps) + " COMPLETED";

    else return "simulateStep "+ std:: to_string(numOfSteps)+" ERROR";
}

SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);
}

//copy constructor
SimulateStep:: SimulateStep(const SimulateStep &other) : BaseAction(other), numOfSteps(other.numOfSteps){}


// Implementation for AddOrder methods

AddOrder::AddOrder(int id) : customerId(id) {}

void AddOrder::act(WareHouse &wareHouse) {
    Customer& customer = wareHouse.getCustomer(customerId);

    if(!customer.canMakeOrder() || customerId<0 || customerId >= wareHouse.getCustomerCounter()){ //check if the order id is legal and the customer 
    //is capable to make an order
        error("Cannot place this order");
    }
    else{ //update the order's members
        int orderId = wareHouse.getOrderCounter();
        int distance = customer.getCustomerDistance();
        Order* order = new Order(orderId ,customerId ,distance);
        wareHouse.addOrder(order);
        complete();
    }
    wareHouse.addAction(this); //push the action to actionsLog
}

string AddOrder::toString() const {
    if (getStatus() == ActionStatus :: COMPLETED)
        return "order "+ std:: to_string(customerId) + " COMPLETED";
    else return "order "+ std:: to_string(customerId)+" ERROR";
}

AddOrder* AddOrder::clone() const {
    return new AddOrder(*this);
}

//copy constructor
AddOrder:: AddOrder(const AddOrder &other) : BaseAction(other), customerId(other.customerId){}

// Implementation for AddCustomer methods

AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders)
    : customerName(customerName),customerType(stringToCustomerType(customerType)), distance(distance), maxOrders(maxOrders){}

const CustomerType AddCustomer:: stringToCustomerType(string stringCustomerType) const{
    if (stringCustomerType == "Soldier") {
        return CustomerType::Soldier;
    } 
    else return CustomerType::Civilian; 
}

const string AddCustomer:: CustomerTypeTostring(CustomerType customerType) const{
    if (customerType == CustomerType::Soldier) {
        return "Soldier";
    } 
    else return "Civilian"; 
}

void AddCustomer::act(WareHouse &wareHouse) {
    int id = wareHouse.getCustomerCounter();    //find id of the customer 
    // create new customer regarding to the customer type
    if (customerType == CustomerType::Soldier){
        SoldierCustomer* customer = new SoldierCustomer(id, customerName, distance, maxOrders);
        wareHouse.addCustomer(customer);
    }
    else{
        CivilianCustomer* customer = new CivilianCustomer(id, customerName, distance, maxOrders);
        wareHouse.addCustomer(customer);
    }
    complete();
    wareHouse.addAction(this); //push the action to actionsLog
}

AddCustomer* AddCustomer::clone() const {
    return new AddCustomer(*this);
}

string AddCustomer::toString() const {
    return "customer "+ customerName + CustomerTypeTostring(customerType) + std:: to_string(distance) + std:: to_string(maxOrders) + " COMPLETED";
}

//copy constructor
AddCustomer:: AddCustomer(const AddCustomer &other) : BaseAction(other), customerName(other.customerName), customerType(other.customerType),
distance(other.distance), maxOrders(other.maxOrders){}

// Implementation for PrintOrderStatus methods

PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) {}

void PrintOrderStatus::act(WareHouse &wareHouse) {
    Order order = wareHouse.getOrder(orderId);
    if (orderId < 0 || orderId >= wareHouse.getOrderCounter())  //cheak if the id is legal
        error("Order doesn't exist");

    else{
        cout << "OrderId: " + std :: to_string(order.getId()) << endl;
        cout << "OrderStatus: " + order.toString() << endl;
        cout << "CusromerID: " + std :: to_string(order.getCustomerId()) <<endl;
        if (order.getCollectorId() == NO_VOLUNTEER)
            cout << "Collector: None" <<endl;
        else
            cout << "Collector: " + std :: to_string(order.getCollectorId()) <<endl;
        if (order.getDriverId() == NO_VOLUNTEER)
            cout << "Driver: None" <<endl;
        else
            cout << "Driver: " + std :: to_string(order.getDriverId()) <<endl;

        complete();
    }

    wareHouse.addAction(this); //push the action to actionsLog
}

PrintOrderStatus* PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const {
    if (getStatus() == ActionStatus :: COMPLETED)
        return "orderStatus "+ std:: to_string(orderId) + " COMPLETED";
    else return "orderStatus "+ std:: to_string(orderId)+" ERROR";
}

//copy constructor
PrintOrderStatus:: PrintOrderStatus(const PrintOrderStatus &other) : BaseAction(other), orderId(other.orderId){}

// Implementation for PrintCustomerStatus methods

PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse) {
    Customer& customer = wareHouse.getCustomer(customerId);
    if(customerId<0 || customerId >= wareHouse.getCustomerCounter()){ //cheak if the id is legal
        error("Customer doesn't exist");
    }
    
    else{
        cout << "CustomerId: " + std :: to_string(customer.getId()) << endl;
        vector<int> customerOrdersId = customer.getOrdersIds();
        // iterate each order that the customer made
        for (size_t i=0; i< customerOrdersId.size(); i++){
            cout << "OrderId: " + std :: to_string(customerOrdersId[i]) << endl;
            Order order = wareHouse.getOrder(customerOrdersId[i]);  //find the order to find the status 
            cout << "OrderStatus: " + order.toString() <<endl;
        }
        int numOrdersLeft = customer.getMaxOrders() - customer.getNumOrders();
        cout << "numOrdersLeft: " + std:: to_string(numOrdersLeft) <<endl;

        complete();
    }
    wareHouse.addAction(this); //push the action to actionsLog
}

PrintCustomerStatus* PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const {
    if (getStatus() == ActionStatus :: COMPLETED)
        return "customerStatus "+ std:: to_string(customerId) + " COMPLETED";
    else return "customerStatus "+ std:: to_string(customerId)+" ERROR";
}

//copy constructor
PrintCustomerStatus:: PrintCustomerStatus(const PrintCustomerStatus &other) : BaseAction(other), customerId(other.customerId){}

// Implementation for PrintVolunteerStatus methods

PrintVolunteerStatus::PrintVolunteerStatus(int id) : VolunteerId(id) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    Volunteer& volunteer = wareHouse.getVolunteer(VolunteerId);
    if(volunteer.getName() == "errorVolunteer"){  //cheak if the volunteer not the error volunteer 
        error("Volunteer doesn't exist");
    }

    else{
        cout << "VolunteerId: " + std :: to_string(volunteer.getId()) << endl;
        string typeVol = volunteer.toString();

        if(volunteer.isBusy()){  
            cout << "isBusy: True" << endl;
            cout << "OrderID: " + std:: to_string(volunteer.getActiveOrderId()) <<endl;

            if (typeVol == "CollectorVolunteer" || typeVol == "LimitedCollectorVolunteer"){
                CollectorVolunteer* collVul = static_cast<CollectorVolunteer*>(&volunteer); //Casting is done in order to use the function getTimeLeft() 
                cout << "TimeLeft: " + std:: to_string(collVul->getTimeLeft()) << endl;
            }

            else {
                DriverVolunteer* drivVul = static_cast<DriverVolunteer*>(&volunteer);  //Casting is done in order to use the function getDistanceLeft() 
                cout << "DistanceLeft: " + std:: to_string(drivVul->getDistanceLeft()) << endl;
            }
        }

        else{ 
            cout << "isBusy: False" << endl;
            cout << "OrderID: None" << endl;
            cout << "TimeLeft: None" << endl;
        }


        if (typeVol == "LimitedCollectorVolunteer"){ 
            LimitedCollectorVolunteer* limCollVul = static_cast<LimitedCollectorVolunteer*>(&volunteer); //Casting is done in order to use the function getNumOrdersLeft() 
            cout << "OrdersLeft: " + std:: to_string(limCollVul->getNumOrdersLeft()) << endl;
        }

        else if (typeVol == "LimitedDriverVolunteer"){
            LimitedDriverVolunteer* limDrivVul = static_cast<LimitedDriverVolunteer*>(&volunteer); //Casting is done in order to use the function getNumOrdersLeft()
            cout << "OrdersLeft: " + std:: to_string(limDrivVul->getNumOrdersLeft()) << endl;
        }
        else cout << "OrdersLeft: No Limit" << endl;

        complete();
    }
    wareHouse.addAction(this); //push the action to actionsLog
}

PrintVolunteerStatus* PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const {
    if (getStatus() == ActionStatus :: COMPLETED)
        return "volunteerStatus "+ std:: to_string(VolunteerId) + " COMPLETED";
    else return "volunteerStatus "+ std:: to_string(VolunteerId)+" ERROR";
}

//copy constructor
PrintVolunteerStatus:: PrintVolunteerStatus(const PrintVolunteerStatus &other) : BaseAction(other), VolunteerId(other.VolunteerId){}

// Implementation for PrintActionsLog methods

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(WareHouse &wareHouse) {
    vector<BaseAction*> actions = wareHouse.getActions();
    for(size_t i=0; i<actions.size(); i++){  //iterate every action and ptint it
        cout << actions[i]->toString() << endl;
    }
    complete();
    wareHouse.addAction(this); //push the action to actionsLog
}

PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const {
    return "log COMPLETED";  
}

//copy constructor
PrintActionsLog:: PrintActionsLog(const PrintActionsLog &other) : BaseAction(other){}

// Implementation for Close methods

Close::Close() {}

void Close::act(WareHouse &wareHouse) {
    wareHouse.close(); 
    complete();
    wareHouse.addAction(this); //push the action to actionsLog
}

Close* Close::clone() const {
    return new Close(*this);
}

string Close::toString() const {
    return "close COMPLETED";
}

//copy constructor
Close:: Close(const Close &other) : BaseAction(other){}

// Implementation for BackupWareHouse methods

BackupWareHouse::BackupWareHouse() {}

void BackupWareHouse::act(WareHouse &wareHouse) {
    if(backup != nullptr){
        delete backup; 
    }
    backup = new WareHouse(wareHouse); 
    complete();
    wareHouse.addAction(this); //push the action to actionsLog
}

BackupWareHouse* BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const {
    return "backup COMPLETED";
}

//copy constructor
BackupWareHouse:: BackupWareHouse(const BackupWareHouse &other) : BaseAction(other){}

// Implementation for RestoreWareHouse methods

RestoreWareHouse::RestoreWareHouse() {}

void RestoreWareHouse::act(WareHouse &wareHouse) {
    if(backup == nullptr)
        error("No backup available");   
    else {
        wareHouse = *backup;
        complete();       
    }
    wareHouse.addAction(this); //push the action to actionsLog
}

RestoreWareHouse* RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const {

    if (getStatus() == ActionStatus :: COMPLETED)
        return "restore COMPLETED";
    else return "restore ERROR";
}

//copy constructor
RestoreWareHouse:: RestoreWareHouse(const RestoreWareHouse &other) : BaseAction(other){}