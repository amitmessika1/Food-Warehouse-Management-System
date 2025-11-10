#include "Volunteer.h"
#include <iostream>


Volunteer::Volunteer(int id, const string &name) : completedOrderId(NO_ORDER), activeOrderId(NO_ORDER),id(id), name(name) {}

int Volunteer::getId() const {
   return id;
}

const string &Volunteer::getName() const {
    return name;
}

int Volunteer::getActiveOrderId() const {
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const {
    return completedOrderId;
}

bool Volunteer::isBusy() const {
    if (activeOrderId == NO_ORDER)
        return false;
    return true; 
}

//copy constructor
Volunteer::Volunteer(const Volunteer &other)
    :completedOrderId(other.completedOrderId), activeOrderId(other.activeOrderId), id(other.id), name(other.name) {}


//CollectorVolunteer methods

CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown) : Volunteer(id, name), coolDown(coolDown), timeLeft(coolDown) {}

CollectorVolunteer* CollectorVolunteer::clone() const {
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step() {
    if (timeLeft==0){}  //do nothing
    else{
        timeLeft--;
        if(timeLeft==0){  //finished processing the order - update the members
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }
}

int CollectorVolunteer::getCoolDown() const {
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const {
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown() {
    if (timeLeft==0){} //do nothing
    else{
        timeLeft--;
        if(timeLeft==0){
           return true;
        }        
    }
    return false;
}

bool CollectorVolunteer::hasOrdersLeft() const {
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const {

    if (order.getStatus() == OrderStatus::PENDING && !isBusy()) //cheak if the order is in the right status and he is free
        return true;
    return false;
}

void CollectorVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    timeLeft = coolDown;
}

string CollectorVolunteer::toString() const {
    return "CollectorVolunteer";
}

//copy constructor
CollectorVolunteer::CollectorVolunteer(const CollectorVolunteer &other)
    :Volunteer(other) , coolDown(other.coolDown),timeLeft(other.timeLeft){}



// Implementations for LimitedCollectorVolunteer methods

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown, int maxOrders)
    : CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const {
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const {
    if (ordersLeft != 0)
        return true;
    return false;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {
    if (order.getStatus() == OrderStatus::PENDING && !isBusy() && hasOrdersLeft()) //cheak if the order is in the right status and he is free and capable
        return true;
    return false;
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order) {
    CollectorVolunteer:: acceptOrder(order);
    if (ordersLeft==0){} //do nothing
    else ordersLeft--;
}

int LimitedCollectorVolunteer::getMaxOrders() const {
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const {
    return "LimitedCollectorVolunteer";
}

//copy constructor
LimitedCollectorVolunteer::LimitedCollectorVolunteer(const LimitedCollectorVolunteer &other)
    :CollectorVolunteer(other) , maxOrders(other.maxOrders),ordersLeft(other.ordersLeft){}


// Implementations for DriverVolunteer methods

DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep)
    : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft() {}

DriverVolunteer* DriverVolunteer::clone() const {
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const {
    return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const {
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const {
    return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft() {
    distanceLeft = distanceLeft - distancePerStep;
    if (distanceLeft<=0)
        return true;
    else
        return false;
}

bool DriverVolunteer::hasOrdersLeft() const {
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const {
    if (order.getStatus() == OrderStatus::COLLECTING && !isBusy() && (order.getDistance()<= maxDistance)) //cheak if the order is in the right status and he is free and capable
        return true;
    return false;
}

void DriverVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
}

void DriverVolunteer::step() {
    if (distanceLeft==0){} //do nothing
    else{
        decreaseDistanceLeft();
        if(distanceLeft<=0){ //finished processing the order - update the members
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }
}

string DriverVolunteer::toString() const {
    return "DriverVolunteer";
}

//copy constructor
DriverVolunteer::DriverVolunteer(const DriverVolunteer &other)
    :Volunteer(other) , maxDistance(other.maxDistance),distancePerStep(other.distancePerStep), distanceLeft(other.distanceLeft){}


// Implementations for LimitedDriverVolunteer methods

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders)
    : DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const {
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const {
     if (ordersLeft != 0)
        return true;
    return false;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const {
    if ( DriverVolunteer::canTakeOrder(order) && hasOrdersLeft())
        return true;
    return false;
}

void LimitedDriverVolunteer::acceptOrder(const Order &order) {
    DriverVolunteer:: acceptOrder(order);
    if (ordersLeft==0){} //do nothing
    else ordersLeft--;
}

string LimitedDriverVolunteer::toString() const {
    return "LimitedDriverVolunteer";
}

//copy constructor
LimitedDriverVolunteer::LimitedDriverVolunteer(const LimitedDriverVolunteer &other)
    :DriverVolunteer(other) , maxOrders(other.maxOrders),ordersLeft(other.ordersLeft){}