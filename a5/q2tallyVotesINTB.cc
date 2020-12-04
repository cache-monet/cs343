#include "q2printer.h"
#include "q2tallyVotes.h"
#include "q2voter.h"
#include <iostream>

TallyVotes::TallyVotes(
   unsigned int voters,
   unsigned int group,
   Printer & printer
) : voters(voters), group(group), printer(printer), serving(group) {} // TallyVotes

void TallyVotes::wait() {
    bench.wait();                            // wait until signalled
    while ( rand() % 2 == 0 ) {              // multiple bargers allowed
        try {
            _Accept( vote, done ) {          // accept barging callers
            } _Else {                        // do not wait if no callers
            } // _Accept
        } catch( uMutexFailure::RendezvousFailure & ) {}
    } // while
}

void TallyVotes::signalAll() {               // also useful
    while ( ! bench.empty() ) bench.signal();// drain the condition
}

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
    if (voters < group ) _Throw Failed(); // check if there's enough voters

    unsigned int curTicket = ticket; // assign ticket
    ticket++;
    if ( curTicket >= serving ) {
        bargers++; // increment barger count
        for (;;) { // barger must wait until everyone in current group goes on tour
            if (curTicket < serving) break; // previous group finished
            #ifndef NOOUTPUT
                printer.print( id, Voter::States::Barging, bargers, currentGroup); 
            #endif
            wait();
        }
        bargers--;
    }

    #ifndef NOOUTPUT
        printer.print(id, Voter::States::Vote, ballot); // print ballot
    #endif

    // collect votes
    pictureVotes += ballot.picture;
    statueVotes += ballot.statue;
    giftshopVotes += ballot.giftshop;
    waitingVoters++;

    if (waitingVoters == group )
    { // full tour
        tally(); // count the collected votes
        Tour tour; tour.tourkind = destination; tour.groupno = currentGroup;
        #ifndef NOOUTPUT
            printer.print(id, Voter::States::Complete, tour); // create group
        #endif
        signalAll(); // wake everyone up
    }
    else
    { // wait for others to vote and print blocked and unblock msgs
        #ifndef NOOUTPUT
            printer.print(id, Voter::States::Block, waitingVoters);
        #endif
        wait();
        #ifndef NOOUTPUT
            printer.print(id, Voter::States::Unblock, waitingVoters-1);
        #endif
    }
    waitingVoters--;

    if (voters < group ) _Throw Failed(); // check if there's enough voters after waiting up

    if (waitingVoters == 0) { // allow the next group be formed
        serving += group;
        signalAll();
    }

    Tour tour; tour.tourkind = destination; tour.groupno = currentGroup;
    return tour;
}

void TallyVotes::done() {
    voters--;
    if (voters < group) signalAll();  
}
