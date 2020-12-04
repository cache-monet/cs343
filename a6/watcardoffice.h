#ifndef WATCardOffice_H
#define WATCardOffice_H

#include <queue>
#include "bank.h"
#include "printer.h"
#include "watcard.h"

_Task WATCardOffice {
    struct Job {                              // marshalled arguments and return future
      unsigned int sid;
      unsigned int amount;
      WATCard* watcard;
      WATCard::FWATCard result;             // return future
      Job( unsigned int sid, unsigned int amount, WATCard * watcard ) : sid(sid), amount(amount),  watcard(watcard) {}
    };
    Printer& prt;
    Bank& bank;
    unsigned int numCouriers;

    bool terminating = false; // true when office is deleting
    std::queue<Job*> jobs; // internal buffer of jobs

    void main();
    _Task Courier { 
      unsigned int id; // courier id (place in queue)
      WATCardOffice& office;
      Printer& prt;
      Bank& bank;
      Printer& printer;
      void main();
     public:
      Courier(unsigned int id, WATCardOffice& office, Printer& printer, Bank& bank);
    };                    // communicates with bank
    Courier** couriers; // pool of couriers

  public:
    _Event Lost {};                           // lost WATCard
    WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card );
    Job * requestWork();
};

#endif