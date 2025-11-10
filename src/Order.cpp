#include "Order.h"

Order::Order(int id, int customerId, int distance): id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING),
 collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER){
    // TODO: Implement the constructor
}

int Order::getId() const {
    return id;
}

int Order::getCustomerId() const {
    return customerId;
}

void Order::setStatus(OrderStatus status) {
    this->status = status;
}

void Order::setCollectorId(int collectorId) {
    this->collectorId = collectorId;
}

void Order::setDriverId(int driverId) {
    this->driverId = driverId;
}

int Order::getCollectorId() const {
    return collectorId;
}

int Order::getDriverId() const {
    return driverId;
}

OrderStatus Order::getStatus() const {
    return status;
}

const string Order::toString() const {
    
    if (status == OrderStatus::PENDING)
        return "PENDING";
    if (status == OrderStatus::COLLECTING)
       return "COLLECTING";
    if (status == OrderStatus::DELIVERING)
        return "DELIVERING";
    else return "COMPLETED";
}

int Order:: getDistance() const{
    return distance;
 }