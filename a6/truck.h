#ifndef TRUCK_H
#define TRUCK_H

#include "printer.h"
#include "nameserver.h"
#include "bottlingplant.h"

_Task Truck {
    static const unsigned int flavours = 4; // number of flavours
    Printer & prt;
    NameServer & nameServer;
    BottlingPlant & plant;
    unsigned int numVendingMachines;
    unsigned int maxStockPerFlavour;
    void main();
  public:
    Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};
#endif