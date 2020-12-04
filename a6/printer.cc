#include <iostream>
#include "printer.h"

using namespace std;

Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) :
  numStudents(numStudents), numVendingMachines(numVendingMachines), numCouriers(numCouriers) {
  unsigned int states = numStudents + numVendingMachines + numCouriers + 6;
  buffer = new State[states];

  // print headers 
  cout << "Parent" << '\t' << "Gropoff" << '\t' << "WATOff" << '\t' << "Names" << '\t' << "Truck" << '\t' << "Plant" << '\t';

  for (unsigned int i = 0; i < numStudents; i++) cout << "Stud" << i << '\t';
  for (unsigned int i = 0; i < numVendingMachines; i++) cout << "Mach" << i << '\t';
  for (unsigned int i = 0; i < numCouriers; i++) {
    cout << "Cour" << i;
    if (i != numCouriers -1) cout << '\t';
  }
  cout << endl;
  // print underline
  for (unsigned int i = 0; i < states; i++) {
    // cout << "*******" << (i == states - 1) ? endl : '\t';
    cout << "*******";
    if (i == states - 1) {
      cout << endl; 
    } else {
      cout << '\t';
    }
    // cout << (i < states-1 ? '\t' : endl); // print newline for last thing; else print
    // cout << (i < states-1 ? '\t' : endl); // print newline for last thing; else print
  }
} // Printer::Printer

Printer::~Printer() {
  flush();
  delete[] buffer;
  cout << "***********************" << endl;
} //~printer

void Printer::print( Kind kind, char state ){
  int id = getIndex(kind);
  if (!buffer[id].empty) flush(); // attempting to an non-empty entry trigger flush
  // update entry
  buffer[id].empty = false;
  buffer[id].kind = kind;
  buffer[id].state = state;
}

void Printer::print( Kind kind, char state, unsigned int value1 ){
  int id = getIndex(kind);
  if (!buffer[id].empty) flush(); // attempting to an non-empty entry trigger flush
  // update entry
  buffer[id].empty = false;
  buffer[id].kind = kind;
  buffer[id].state = state;
  buffer[id].value1 = value1;
} // Printer::print

void Printer::print( Kind kind, char state, unsigned int value1, unsigned int value2 ){
  int id = getIndex(kind);
  if (!buffer[id].empty) flush(); // attempting to an non-empty entry trigger flush
  // update entry
  buffer[id].empty = false;
  buffer[id].kind = kind;
  buffer[id].state = state;
  buffer[id].value1 = value1;
  buffer[id].value2 = value2;
} // Printer::print

void Printer::print( Kind kind, unsigned int lid, char state ){
  int id = getIndex(kind, lid);
  if (!buffer[id].empty) flush(); // attempting to an non-empty entry trigger flush
  // update entry
  buffer[id].empty = false;
  buffer[id].kind = kind;
  buffer[id].state = state;
} // Printer::print

void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1 ){
  int id = getIndex(kind, lid);
  if (!buffer[id].empty) flush(); // attempting to an non-empty entry trigger flush
  // update entry
  buffer[id].empty = false;
  buffer[id].kind = kind;
  buffer[id].state = state;
  buffer[id].value1 = value1;
} // Printer::print

void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 ){
  int id = getIndex(kind, lid);
  if (!buffer[id].empty) flush(); // attempting to an non-empty entry trigger flush
  // update entry
  buffer[id].empty = false;
  buffer[id].kind = kind;
  buffer[id].state = state;
  buffer[id].value1 = value1;
  buffer[id].value2 = value2;
} // Printer::print

// return correspending index in printer buffer for each object
int Printer::getIndex(Kind kind, unsigned int lid) {
  switch(kind) {
    case Student:
      return 6 + lid;
    case Vending:
      return 6 + numStudents + lid;
    case Courier:
      return 6 + numStudents + numVendingMachines + lid;
    default:
      return (int)kind; // regular case: index is the enum kind
  }
} // Printer::getIndex

void Printer::flush() {
  for ( unsigned int id = 0; id < 6 + numStudents + numVendingMachines + numCouriers; id++ ) {
    if (id > 0) cout << "\t";
    if (buffer[id].empty) continue; // nothing to print continue to next col

    buffer[id].empty = true; // reset entry status
    char state = buffer[id].state;
    Kind kind = buffer[id].kind;
    // print entry
    cout << state;
    if ( kind == Parent && state == 'D' ) {
      cout << buffer[id].value1 << "," << buffer[id].value2;
    }
    if ( kind == Groupoff && state == 'D' ) {
      cout << buffer[id].value1;
    }
    if ( kind == WATCardOffice && (state == 'C' || state == 'T') ) {
      cout << buffer[id].value1 << "," << buffer[id].value2;
    }
    if ( kind == NameServer) {
      if (state == 'R') cout << buffer[id].value1;
      if (state == 'N') cout << buffer[id].value1 << "," << buffer[id].value2;
    }
    if ( kind == Truck) {
      if (state == 'P') cout << buffer[id].value1;
      if (state == 'd' || state == 'U' || state == 'D') cout << buffer[id].value1 << "," << buffer[id].value2;
    }
    if ( kind == BottlingPlant) {
      if (state == 'G') cout << buffer[id].value1;
    }
    if ( kind == Student) {
      if (state == 'V') cout << buffer[id].value1;
      if (state == 'S' || state == 'G' || state == 'a' || state == 'B' || state == 'A') cout << buffer[id].value1 << "," << buffer[id].value2;
    }
    if ( kind == Vending) {
      if (state == 'S') cout << buffer[id].value1;
      if (state == 'B') cout << buffer[id].value1 << "," << buffer[id].value2;
    }
    if ( kind == Courier) {
      if (state == 'L') cout << buffer[id].value1;
      if (state == 't' || state == 'T') cout << buffer[id].value1 << "," << buffer[id].value2;
    }
  }
  cout << endl;
} // Printer::flush
