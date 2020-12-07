#ifndef BOTTLE_H
#define BOTTLE_H
#include "printer.h"
#include "nameserver.h"

_Task BottlingPlant {
    Printer & prt;
    NameServer & nameServer;
    static const unsigned int flavours = 4; // number of flavours
    unsigned int numVendingMachines;
    unsigned int maxShippedPerFlavour;
    unsigned int maxStockPerFlavour;
    unsigned int timeBetweenShipments;
    bool shutdown = false; // factory shutdowned
    unsigned int supply[flavours]; // tracks supply for each flavor
    void main();
  public:
    _Event Shutdown {};                       // shutdown plant
    BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    void getShipment( unsigned int cargo[] );
};

#endif