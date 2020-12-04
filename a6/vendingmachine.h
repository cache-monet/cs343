#ifndef VENDING_H
#define VENDING_H

#include "printer.h"
#include "nameserver.h"
#include "watcard.h" 

_Task VendingMachine {
    static const unsigned int flavors = 4; // number of flavors
    Printer & prt;
    NameServer & nameServer;
    unsigned int id;
    unsigned int sodaCost;
    unsigned int inventory[flavors]; // tracks inventory for each flavor
    bool restocking = false; // truck is restocking machine
    void main();
  public:
    enum Flavours { Blues, Classic, Rock, Jazz }; // flavours of soda
    _Event Free {};                           // free, advertisement
    _Event Funds {};                          // insufficient funds
    _Event Stock {};                          // flavour out of stock
    VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost );
    void buy( Flavours flavour, WATCard & card );
    unsigned int * inventory();
    void restocked();
    _Nomutex unsigned int cost() const;
    _Nomutex unsigned int getId() const;
};
