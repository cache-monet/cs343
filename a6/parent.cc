#include "parent.h"
#include "MPRNG.h"

extern MPRNG mprng;

Parent::Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay ) :
  prt(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {} // Printer::Printer 

void Parent::main() {
  prt.print(Printer::Parent, 'S');
  yield(100); // yield 100 times after starting
  for (;;) {
    _Accept(~Parent) {
      break;
    } _Else {
      yield(parentalDelay); // deplay before each transfer
      unsigned int sid = mprng(numStudents-1); // student who gets the money
      unsigned int gift = mprng(minAmount, maxAmount); // amount to give to student
      prt.print(Printer::Parent, 'D', sid, gift); // terminate task
      bank.deposit(sid, gift);
    }
  }
  prt.print(Printer::Parent, 'F'); // terminate task
} // Parent::main