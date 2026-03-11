# Food Warehouse Management System

A C++ simulation of a warehouse order management system developed as part of a **Systems Programming** course.

The program models how a food warehouse handles incoming orders using volunteers with different roles.  
Customers place orders, collectors prepare packages, and drivers deliver them through a step-based simulation engine.

The project demonstrates **object-oriented design, memory management, and system simulation in C++**.
---

# Overview

This system simulates the workflow of a food warehouse responsible for delivering food packages.

The system includes:

- Customers that place orders
- Collectors that prepare packages
- Drivers that deliver orders
- A warehouse manager that controls the entire simulation

The program progresses in **discrete simulation steps**, updating volunteers and orders accordingly.

---

# Architecture

The system follows an **Object-Oriented architecture** where the `Warehouse` acts as the central controller:

```
                     +----------------+
                     |    Warehouse   |
                     +----------------+
                    /        |        \
                   /         |         \
          +-----------+  +-----------+  +-----------+
          | Customers |  | Volunteers|  |  Orders   |
          +-----------+  +-----------+  +-----------+
                |               |
        -----------------   -----------------
        |               |   |               |
+---------------+ +---------------+ +---------------+ +---------------+
| CivilianCust. | | SoldierCust.  | |   Collectors  | |    Drivers    |
+---------------+ +---------------+ +---------------+ +---------------+
```
 
The system is designed using **inheritance and polymorphism**:
```
Volunteer (abstract)
│
├── Collector
│     └── LimitedCollector
│
└── Driver
      └── LimitedDriver
```
---

# Core Components

## Warehouse

Central system controller.

Responsibilities:

- Manage customers
- Manage volunteers
- Store and update orders
- Execute simulation steps
- Maintain action history

Orders are divided into three lists:

- Pending Orders
- In Process Orders
- Completed Orders

---

## Customer

Represents a warehouse client that can place orders.

Customer types:

- Soldier
- Civilian

Attributes:

- unique ID
- distance from warehouse
- maximum allowed orders

---

## Volunteer

Abstract base class representing warehouse workers.

Volunteer types:

### Collector
Responsible for preparing orders.

### Limited Collector
Collector with a limit on the number of orders they can process.

### Driver
Responsible for delivering prepared orders.

### Limited Driver
Driver with a limit on the number of deliveries.

Each volunteer processes work during simulation steps.

---

## Order

Represents a food order.

Each order includes:

- Customer ID
- Collector ID
- Driver ID
- Order status

Order lifecycle:
PENDING → COLLECTING → DELIVERING → COMPLETED

---

## BaseAction

Abstract class representing user commands.

Examples of supported actions:

- AddCustomer
- AddOrder
- SimulateStep
- PrintOrderStatus
- PrintCustomerStatus
- PrintVolunteerStatus
- BackupWarehouse
- RestoreWarehouse
- Close

Each executed action is stored in an **actions log**.

---

# Simulation Flow

Customer places order
↓
Order added to Pending Orders
↓
Collector processes order
↓
Driver delivers order
↓
Order marked as Completed

Simulation progresses using:
step <number_of_steps>

Each step updates:

- collector processing time
- driver distance remaining
- volunteer availability
- order status
  
---

# Build and Run

Compile using `make`:
make

Compiler flags:
-g -Wall -Weffc++ -std=c++11

Executable location:
bin/warehouse

Run the program:
bin/warehouse <config_file>

Example:
bin/warehouse input.txt

---

## Project Structure
```
src/
    Action.cpp
    Customer.cpp
    Order.cpp
    Volunteer.cpp
    Warehouse.cpp
    Parse.cpp

include/
    Action.h
    Customer.h
    Order.h
    Volunteer.h
    Warehouse.h

bin/

makefile
```
---

## Technologies

- C++
- Object-Oriented Programming (OOP)
- Memory Management
- Rule of Five
- Makefile
- Linux / Unix development environment
- Valgrind (memory leak detection)




    
