#include <iostream>
#include <string>

#include "config.h"
#include "MPRNG.h"

#include "bank.h"
#include "bottlingplant.h"
#include "groupoff.h"
#include "nameserver.h"
#include "parent.h"
#include "printer.h"
#include "vendingmachine.h"
#include "watcardoffice.h"
#include "student.h"

using namespace std;

MPRNG mprng; // globally defined mprng


int main( int argc, char *argv[] ) {
  string configFile = "soda.config"; // default configFile
  int seed = getpid(); // default seed value
  try {
    switch(argc) {
      case 3: seed = stoi(argv[2]); if (seed <= 0) throw 1;
      case 2: configFile = argv[1];
      case 1: break;
      default: throw 1;
    }
  } catch (...) {
    cout << "Usage: " << argv[0] << "config-file [ random-seed (> 0) ] ]" << endl;
    exit(1);
  } // try

  mprng.set_seed( seed );

  ConfigParms params;
  processConfigFile(configFile.c_str(), params );
  uProcessor p[1] __attribute__(( unused )); // add another processor for parallelism

  Printer prt(params.numStudents, params.numVendingMachines, params.numCouriers);
  Bank bank(params.numStudents);
  Parent parent(prt, bank, params.numStudents, params.parentalDelay);
  WATCardOffice office(prt, bank, params.numCouriers);

  Groupoff groupoff(prt, params.numStudents, params.sodaCost, params.groupoffDelay);
  NameServer nameServer(prt, params.numVendingMachines, params.numStudents);

  BottlingPlant* plant = new BottlingPlant(
    prt,
    nameServer,
    params.numVendingMachines,
    params.maxShippedPerFlavour,
    params.maxStockPerFlavour,
    params.timeBetweenShipments
  );

  VendingMachine** vms = new VendingMachine* [params.numVendingMachines]; 
  for (unsigned int i = 0; i < params.numVendingMachines; i++) vms[i] = new VendingMachine(prt, nameServer, i, params.sodaCost);

  Student** students = new Student* [params.numStudents];
  for (unsigned int i = 0; i < params.numStudents; i++) {
    students[i] = new Student(prt, nameServer, office, groupoff, i, params.maxPurchases);
  }
  // // wait for students to finish
  for (unsigned int i = 0; i < params.numStudents; i++) delete students[i];
  delete[] students;

  // delete plant before vending machine so truck can complete the final deliveries
  delete plant;

  for (unsigned int i = 0; i < params.numVendingMachines; i++) delete vms[i];
  delete[] vms;

  return 0;
}