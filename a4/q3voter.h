#ifndef VOTER_H
#define VOTER_H
#include "q3tallyVotes.h"

_Monitor Printer;

_Task Voter {
    unsigned int id; // voter id
    unsigned int nvotes;
    TallyVotes& voteTallier; // current vote tallier
    Printer& printer; // printer object
    TallyVotes::Ballot cast();
    void main();
  public:
    enum States { Start = 'S', Vote = 'V', Block = 'B', Unblock = 'U', Barging = 'b',
         Done = 'D', Complete = 'C', Going = 'G', Failed = 'X', Terminated = 'T' };
    Voter( unsigned int id, unsigned int nvotes, TallyVotes & voteTallier, Printer & printer );
};
#endif