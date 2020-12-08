#ifndef NAMESERVER_H
#define NAMESERVER_H

#include "printer.h"
#include "vendingmachine.h"
#include <vector>

_Task VendingMachine; // forward declare VendingMachine

_Task NameServer {
    Printer & prt;
    unsigned int numVendingMachines;
    unsigned int numStudents;
    VendingMachine** machines; // array of vending machines
    unsigned int * assignedMachines;// assigned machines for each student
    unsigned int registeredMachines = 0; // track how many machines are currently registered
    unsigned int sid; // student requesting a new machine
    void main();
  public:
    NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
    void VMregister( VendingMachine * vendingmachine );
    VendingMachine * getMachine( unsigned int id );
    VendingMachine ** getMachineList();
};

#endif