#include "student.h"
#include "vendingmachine.h"
#include "watcard.h"
#include "MPRNG.h"

extern MPRNG mprng;

Student::Student(
  Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
  unsigned int id, unsigned int maxPurchases
) : 
  prt(prt),
  nameServer(nameServer),
  cardOffice(cardOffice),
  groupoff(groupoff),
  id(id),
  maxPurchases(maxPurchases) {} // Student::Student

void Student::main() {
  unsigned int purchases = mprng(1, maxPurchases);  // # number of sodas to bu
  VendingMachine::Flavours favourite = (VendingMachine::Flavours) mprng(flavours - 1); // pick a random fav flavour
  prt.print(Printer::Student, id, 'S', favourite, purchases);
  // create $5 watcard and gift card
  WATCard::FWATCard giftcard = groupoff.giftCard();
  WATCard::FWATCard watcard = cardOffice.create(id, 5);
  // select machine
  VendingMachine* vm = nameServer.getMachine(id);
  prt.print(Printer::Student, id, 'V', vm->getId());
  // buy some sodas!
  PurchaseSodas: for (unsigned int i = 0; i < purchases; i++) {
    yield(mprng(1, 10));
    // keep trying until we buy a soda
    Buy: for ( ;; ) {
      _Select(giftcard) { // use giftcard first if it's available
        try {
          vm->buy(favourite, *giftcard());
          prt.print(Printer::Student, id, 'G', favourite, giftcard()->getBalance());
          giftcard.reset();
          break Buy; // soda bought let's skedaddle
        }
        catch (VendingMachine::Free) {
          prt.print(Printer::Student, id, 'a', favourite, giftcard()->getBalance());
          yield(4); // watch ad
          break Buy; // purchase complete
        }
        catch (VendingMachine::Stock) {
          vm = nameServer.getMachine(id); // get another machine
          prt.print(Printer::Student, id, 'V', vm->getId());
        } // try
      } or _Select(watcard) {
        try {
          vm->buy(favourite, *watcard());
          prt.print(Printer::Student, id, 'B', favourite, watcard()->getBalance());
          break Buy; // soda bought let's skedaddle
        } catch (VendingMachine::Free) {
          prt.print(Printer::Student, id, 'A', favourite, watcard()->getBalance());
          yield(4); // watch ad
          break Buy; // purchase complete
        }
        catch (VendingMachine::Stock) {
          vm = nameServer.getMachine(id); // get another machine
          prt.print(Printer::Student, id, 'V', vm->getId());
        }
        catch (VendingMachine::Funds) {
          unsigned int amount = 5 + vm->cost();
          watcard = cardOffice.transfer(id, amount, watcard()); // transfer money to watcard and try again
        }
        catch (WATCardOffice::Lost) {
          prt.print(Printer::Student, id, 'L');
          delete watcard();
          watcard = cardOffice.create(id, 5); // get a replacement card and reattempt purchase
        } // try
      } // _Select
    } // Buy
  } // PurchaseSodas
  delete watcard();
  prt.print(Printer::Student, id, 'F');
} // Student::main