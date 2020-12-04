#ifndef PARENT_H
#define PARENT_H

#include "printer.h"
#include "bank.h"

_Task Parent {
    Printer & prt;
    Bank & bank;
    unsigned int numStudents;
    unsigned int parentalDelay;
    static const unsigned int minAmount = 1;
    static const unsigned int maxAmount = 3;
    void main();
  public:
    Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay );
};

#endif