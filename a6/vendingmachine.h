#ifndef VENDING_H
#define VENDING_H

#include "printer.h"
#include "nameserver.h"
#include "watcard.h" 

_Task NameServer; // forward declare NameServer

_Task VendingMachine {
    static const unsigned int flavours = 4; // number of flavours
    Printer & prt;
    NameServer & nameServer;
    unsigned int id;
    unsigned int sodaCost;
    unsigned int supply[flavours]; // tracks supply for each flavor
    bool restocking = false; // marks whether restock is in process
    void main();
  public:
    enum Flavours { Blues=0, Classic, Rock, Jazz }; // flavours of soda
    _Event Free {};                           // free, advertisement
    _Event Funds {};                          // insufficient funds
    _Event Stock {};                          // flavour out of stock
    VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost );
    void buy( Flavours flavour, WATCard & card );
    unsigned int * inventory();
    void restocked();
    _Nomutex unsigned int cost() const;
    _Nomutex unsigned int getId() const;
  private:
    Flavours purchased; // flavor purchased
    WATCard* customer; // last student who purchased the flavor
};

#endif