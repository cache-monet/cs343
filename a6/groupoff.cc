#include "groupoff.h"
#include "MPRNG.h"

extern MPRNG mprng;

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ) :
  prt(prt),
  numStudents(numStudents),
  sodaCost(sodaCost),
  groupoffDelay(groupoffDelay)
{
} // Groupoff::Groupoff

WATCard::FWATCard Groupoff::giftCard() {
  WATCard::FWATCard futureGiftCard;
  futures.push_back(futureGiftCard);
  return futureGiftCard;
} // Groupoff::giftCard

void Groupoff::main() {
  prt.print(Printer::Groupoff, 'S');
  for (unsigned i = 0; i < numStudents; i++) {
    _Accept(giftCard); // group off begins by accepting request for a giftcard from each student
  }

  for (unsigned int i = 0; i < numStudents; i++) {
    _Accept(~Groupoff) {
      break;
    } _Else { // fill up giftcard with actual value
      yield(groupoffDelay); // wait to deliver giftcard
      // create watcard with soda cost value
      giftCards.push_back(new WATCard());
      giftCards.back()->deposit(sodaCost);
      // randomly choose one of the unfilled giftcard

      unsigned int idx = mprng(futures.size() - 1);
      futures[idx].delivery(giftCards.back()); // deliver created giftcards
      prt.print(Printer::Groupoff, 'D', giftCards.back()->getBalance());
      futures.erase( futures.begin() + idx); // fulfilled remove undelivered index from undelivered
    } // _Accept
  } // for
  prt.print(Printer::Groupoff, 'F');
} // Groupoff::main