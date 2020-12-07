#include "bottlingplant.h"
#include "MPRNG.h"
#include "truck.h"

extern MPRNG mprng;

BottlingPlant::BottlingPlant(
  Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
  unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments
) : 
  prt(prt),
  nameServer(nameServer),
  numVendingMachines(numVendingMachines),
  maxShippedPerFlavour(maxShippedPerFlavour),
  maxStockPerFlavour(maxStockPerFlavour),
  timeBetweenShipments(timeBetweenShipments)
{
  for (unsigned int i = 0; i < flavours; i++) supply[i] = 0;
} // BottlingPlant::BottlingPlant

void BottlingPlant::getShipment(unsigned int cargo[]) {
  if (shutdown) {
    uRendezvousAcceptor();
    _Throw Shutdown();
  }
  for (unsigned int i = 0; i < flavours; i++) cargo[i] = supply[i];
} // BottlingPlant::getShipment

void BottlingPlant::main() {
  prt.print(Printer::BottlingPlant, 'S');
  Truck* truck = new Truck(prt, nameServer, *this, numVendingMachines, maxStockPerFlavour);
  for (;;) {
    _Accept(~BottlingPlant) {
      shutdown = true;
      break;
    } _Else {
      // start a production
      yield(timeBetweenShipments);
      unsigned int producedSodas = 0;
      for (unsigned int i = 0; i < flavours; i++) {
        supply[i] = mprng(maxShippedPerFlavour);
        producedSodas += supply[i];
      }
      prt.print(Printer::BottlingPlant, 'G', producedSodas);
      _Accept(getShipment) {
        prt.print(Printer::BottlingPlant, 'P');
      }
    }  // _Else
  } // for
  // mark factory as closing
  try {
    _Accept(getShipment); // accept final shipment
  } catch (uMutexFailure::RendezvousFailure) {} // try
  prt.print(Printer::BottlingPlant, 'F');
  if (truck != nullptr) delete truck;
} // BottlingPlant::main