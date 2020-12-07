#include "bank.h"

Bank::Bank(unsigned int numStudents) :
  numStudents(numStudents),
  balances( new unsigned int[numStudents] ) {
    for (unsigned int i = 0; i < numStudents; i++) balances[i] = 0;
} // Bank::Bank

Bank::~Bank() {
  delete[] balances;
} // Bank::~Bank

void Bank::deposit( unsigned int id, unsigned int amount ) {
  balances[id] += amount;
} // Bank::deposit

void Bank::withdraw( unsigned int id, unsigned int amount ) {
  // block until we have enough money in account to withdrawl
  while( balances[id] < amount ) {
    _Accept(Bank::deposit);
  }
  balances[id] -= amount; 
} // Bank::withdraw
