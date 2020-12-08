#ifndef GROUPOFF_H
#define GROUPOFF_H

#include <vector> // using vector because its better for accessing random indices
#include "printer.h"
#include "watcard.h"

_Task Groupoff {
    Printer& prt;
    unsigned int numStudents;
    unsigned int sodaCost;
    unsigned int groupoffDelay;
    std::vector<WATCard::FWATCard> futures;
    std::vector<WATCard*> giftCards; // stores giftcards on heap
    void main();
  public:
    Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    WATCard::FWATCard giftCard();
};

#endif