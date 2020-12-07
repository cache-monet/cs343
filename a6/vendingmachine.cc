#include "vendingmachine.h"
#include "MPRNG.h"

extern MPRNG mprng;

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost ) : 
  prt(prt),
  nameServer(nameServer),
  id(id),
  sodaCost(sodaCost)
{
  for (unsigned int i = 0; i < flavours; i++) supply[i] = 0; // no inventory  to start
} // VendingMachine::VendingMachine

void VendingMachine::buy(Flavours flavour, WATCard& card) {
  purchased = flavour; customer = &card;
  if (supply[(int)flavour] < 1) _Throw Stock(); // check if flavour is in stock
  if (mprng(5-1) == 0) { // 1 / 5 chance soda is free
    supply[(int)flavour]--;
    _Throw Free();
  }
  if (card.getBalance() < sodaCost) _Throw Funds(); // check if student have enough funds
} // VendingMachine::buy

unsigned int* VendingMachine::inventory() {
  return supply;
} // VendingMachine::inventory

void VendingMachine::restocked() {
  return; // don't do anything here
} // VendingMachine::restocked

_Nomutex unsigned int VendingMachine::cost() const {
  return sodaCost;
} // VendingMachine::cost

_Nomutex unsigned int VendingMachine::getId() const {
  return id;
} // VendingMachine::getId


void VendingMachine::main() {
  nameServer.VMregister(this); // begins with registering self with nameserver
  prt.print(Printer::Vending, id, 'S', sodaCost);
  try {
    for (;;) {
      _Accept(~VendingMachine) {
        break;
      } or _Accept(inventory) { // starting restock
        restocking = true;
        prt.print(Printer::Vending, id, 'r');
      } or _Accept(restocked) {
        restocking = false; // restock complete
      } or _When(!restocking) _Accept(buy) { // students can buy only when 
        // this only runs when buy terminates without throwing ie: successful purchase
        customer->withdraw(sodaCost); // debit purchase
        supply[(int)purchased] -= 1; // decrement stock
        prt.print(Printer::Vending, id, 'B', purchased, supply[(int)purchased]);
      }
    } // for
  } catch(uMutexFailure::RendezvousFailure) {} // try
  prt.print(Printer::Vending, id, 'F');
} // VendingMachine::main