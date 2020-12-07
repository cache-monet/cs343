#include "groupoff.h"
#include "MPRNG.h"

extern MPRNG mprng;

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ) :
  prt(prt),
  numStudents(numStudents),
  sodaCost(sodaCost),
  groupoffDelay(groupoffDelay) {
  giftCards = new WATCard*[numStudents];
} // Groupoff::Groupoff

WATCard::FWATCard Groupoff::giftCard() {
  WATCard::FWATCard futureGiftCard;
  futureGiftCards.push_back(futureGiftCard);
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
      WATCard* card = new WATCard;
      card->deposit(sodaCost);
      giftCards[i] = card;
      // randomly choose one of the unfilled giftcard
      unsigned int idx = mprng(futureGiftCards.size() - 1);
      futureGiftCards[idx].delivery(card);
      futureGiftCards.erase( futureGiftCards.begin() + idx ); // fulfilled remove future from list
      prt.print(Printer::Groupoff, 'D', sodaCost);
    }
  }
  prt.print(Printer::Groupoff, 'F');
} // Groupoff::main