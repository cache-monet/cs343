#include "nameserver.h"

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents ) :
  prt(prt),
  numVendingMachines(numVendingMachines),
  numStudents(numStudents)
{
    machines = new VendingMachine*[numVendingMachines];  // allocate space for an array of vending machines
    assignedMachines = new unsigned int[numStudents]; // allocate space for studnet machine assignmen
    // round robin distribution 
    for (unsigned int i = 0; i < numStudents; i++) assignedMachines[i] = i % numVendingMachines;
} // NameServer::NameServer

void NameServer::VMregister(VendingMachine* vendingMachine) {
  machines[registeredMachines] = vendingMachine;
} // NameServer::VMregister


VendingMachine* NameServer::getMachine(unsigned int id) {
  sid = id;
  unsigned int currentMachine = assignedMachines[sid]; // get the machine student is currently assigned to
  return machines[currentMachine];
} // NameServer::getMachine

VendingMachine** NameServer::getMachineList() {
  return machines;
} // NameServer::getMachineList

void NameServer::main() {
  prt.print(Printer::NameServer, 'S');
  for (;;) {
    _Accept(~NameServer) {
      break;
    } or _When(registeredMachines == numVendingMachines) _Accept(getMachine) {
      unsigned int currentMachine = assignedMachines[sid]; // get the machine student is currently assigned to
      prt.print(Printer::NameServer, 'N', sid, machines[currentMachine]->getId());
      assignedMachines[sid] = (currentMachine + 1) % numVendingMachines; // assign student to the next machine
    } or _When(registeredMachines < numVendingMachines) _Accept(VMregister) {
      prt.print(Printer::NameServer, 'R', registeredMachines);
      registeredMachines++;
    } or _When(registeredMachines == numVendingMachines) _Accept(getMachineList) {}
  }
  // cleanup
  delete[] machines;
  delete[] assignedMachines;
  prt.print(Printer::NameServer, 'F');
} // NameServer::main