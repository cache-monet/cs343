#include "watcardoffice.h"
#include "MPRNG.h"

extern MPRNG mprng;

WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ) :
  prt(prt), bank(bank), numCouriers(numCouriers) {
    // create fixed size courier pool
    couriers = new Courier*[numCouriers];
    for (unsigned int i = 0; i < numCouriers; i++) {
      couriers[i] = new WATCardOffice::Courier(i, *this, prt, bank);
    }
} //WATCardOffice::WATCardOffice

void WATCardOffice::main() {
  prt.print(Printer::WATCardOffice, 'S');
  for (;;) {
    _Accept(~WATCardOffice) {
      break;
    } or _Accept(create) {
      Job* added = jobs.back(); 
      prt.print(Printer::WATCardOffice, 'C', added->sid, added->amount);
    } or _Accept(transfer) {
      Job* added = jobs.back(); 
      prt.print(Printer::WATCardOffice, 'T', added->sid, added->amount);
    } or _When(!jobs.empty()) _Accept(requestWork){
      prt.print(Printer::WATCardOffice, 'W');
      jobs.pop(); // remove job
    }
  }
  // shutdown procedure
  terminating = true;
  // cleanup jobs queue (make sure everything's finished)
  while (!jobs.empty()) { delete jobs.front();  jobs.pop(); } // cleanup jobs
  // cleanup couriers
  for (unsigned int i = 0; i < numCouriers; i++) _Accept(requestWork); // terminate couriers
  for (unsigned int i = 0; i < numCouriers; i++) delete couriers[i]; // free up courier memory
  delete[] couriers;
  prt.print(Printer::WATCardOffice, 'F');
} // WATCardOffice::main

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
  WATCard* card = new WATCard();
  Job* job = new Job(sid, amount, card);
  jobs.push(job); // create and add job to queue
  return job->result; // return future
} // WATCardOffice::create

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) {
  Job* job = new Job(sid, amount, card);
  jobs.push(job); // create and add job to queue
  return job->result; // return future
} // WATCardOffice::transfer

WATCardOffice::Job* WATCardOffice::requestWork() {
  if (terminating) return NULL; // shutdown in process no more jobs to do

  Job* job = jobs.front();
  return job;
} //WATCardOffice::requestWork

// COURIER IMPLEMENTATION
WATCardOffice::Courier::Courier(unsigned int id, WATCardOffice& office, Printer& prt, Bank& bank):
  id(id), office(office), prt(prt), bank(bank) {} // Courier::Courier

void WATCardOffice::Courier::main() {
  prt.print(Printer::Courier, id, 'S');
  for (;;) {
    _Accept(~Courier) {
      break;
    } _Else {
      Job* job = office.requestWork();
      if (job == NULL) break; // No more jobs left stop task

      // get money from bank and put it in the whatcard
      bank.withdraw(job->sid, job->amount);
      job->watcard->deposit(job->amount);
      prt.print(Printer::Courier, id, 'T', job->sid, job->amount);
      //  1/6 chance of courier losing card
      if (mprng(6 - 1) == 0) { 
        prt.print(Printer::Courier, id, 'L', job->sid);
        delete job->watcard;
        job->result.exception( new WATCardOffice::Lost() ); // insert exception into future
      } else {
        job->result.delivery(job->watcard); // deliver watcard to future
      }
      delete job; // cleanup job isn't needed anymore
    }
  }
  prt.print(Printer::Courier, id, 'F');
} // Courier::main