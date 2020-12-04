#ifndef GROUPOFF_H
#define GROUPOFF_H

#include "printer.h"
#include "watcard.h"

_Task Groupoff {
    Printer & prt;
    unsigned int numStudents;
    unsigned int sodaCost;
    unsigned int groupoffDelay;
    void main();
  public:
    Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    ~Groupoff();
    WATCard::FWATCard giftCard();
};

#endif