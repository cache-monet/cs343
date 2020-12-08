#include "truck.h"

#include "MPRNG.h"
#include "vendingmachine.h"

extern MPRNG mprng;

Truck::Truck(
  Printer & prt, NameServer & nameServer, BottlingPlant & plant,
  unsigned int numVendingMachines, unsigned int maxStockPerFlavour
) : 
  prt(prt),
  nameServer(nameServer),
  plant(plant),
  numVendingMachines(numVendingMachines),
  maxStockPerFlavour(maxStockPerFlavour) {} //Truck::Truck

void Truck::main() {
  prt.print(Printer::Truck, 'S');
  VendingMachine** machines = nameServer.getMachineList(); // get location of vending machine
  unsigned int cargo[flavours]; // initialize truck cargo
  unsigned int curVM = 0; // track the next vm to restock
  Main: for (;;) {
    yield(mprng(1, 10)); // coffee break
    // get soda from factory
    for (unsigned int i = 0; i < flavours; i++) cargo[i] = 0; // dump expired cargo
    try {
      plant.getShipment(cargo);
    } catch (BottlingPlant::Shutdown) {
      break Main;
    }
    unsigned int totalCargo = 0;
    for (unsigned int i = 0; i < flavours; i++) totalCargo += cargo[i];
    prt.print(Printer::Truck, 'P', totalCargo);

    // attempt a cycle of restocking vending machines 
    RestockCycle: for (unsigned int VMsRestocked = 0; VMsRestocked < numVendingMachines; VMsRestocked++) {
      VendingMachine* vm = machines[curVM];
      unsigned int vmId = vm->getId();
      prt.print(Printer::Truck, 'd', vmId, totalCargo);

      // restock flavors
      unsigned int unreplenished = 0; // how much soda isn't fully stock
      unsigned int* VMInventory = vm->inventory();
      for (unsigned int i = 0; i < flavours; i++) {
        unsigned int restockAmount = std::min(maxStockPerFlavour - VMInventory[i], cargo[i]);
        // transfer soda from soda to vm
        VMInventory[i] += restockAmount; 
        cargo[i] -= restockAmount; totalCargo -= restockAmount;
        unreplenished += maxStockPerFlavour - VMInventory[i];
      }
      vm->restocked(); // signal vm restock is complete
      if (unreplenished > 0) {
        prt.print(Printer::Truck, 'U', vmId, unreplenished);
      }
      prt.print(Printer::Truck, 'D', vmId, totalCargo);

      // 1/100 chance of flat tire 
      if (mprng(1 - 100) == 0) {
        prt.print(Printer::Truck, 'X');
        yield(10);
      }
      curVM = (curVM + 1) % numVendingMachines;
      if (totalCargo == 0) break RestockCycle; // no more cargo creak cycle
    } // for
  } // for
  prt.print(Printer::Truck, 'F');
}