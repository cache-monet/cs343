#include <iostream>
#include <string>
#include "config.h"
#include "MPRNG.h"
#include "bank.h"
#include "parent.h"
#include "printer.h"

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

  Printer printer(params.numStudents, params.numVendingMachines, params.numCouriers);
  Bank bank(params.numStudents);
  Parent(printer, bank, params.numStudents, params.parentalDelay);
  return 0;
}