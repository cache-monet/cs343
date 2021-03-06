#include <stdlib.h>     /* abs */
#include "q3printer.h"
#include "q3tallyVotes.h"
#include "q3voter.h"

TallyVotes::TallyVotes(
   unsigned int voters,
   unsigned int group,
   Printer & printer
) : muxSem(1), syncSem(0), voters(voters), group(group), printer(printer) {} // syncSem starts as a locked binary semaphore

void TallyVotes::tally() {
    // determine which tour got the most votes
    if ( giftshopVotes >= pictureVotes &&  giftshopVotes >= statueVotes ) {
        destination = GiftShop;   
    } else if ( pictureVotes >= statueVotes) {
        destination = Picture;
    } else {
        destination = Statue;
    }
    pictureVotes = 0, statueVotes = 0, giftshopVotes = 0; // reset vote counter
    currentGroup++;
}

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
    if (voters < group ) _Throw Failed(); // check if there's enough remaning voters to create a group
  muxSem.P(); // begin critical section

    #ifdef NOOUTPUT
        #define PRINT(...)
    #else 
        printer.print(id, Voter::States::Vote, ballot); // print ballot
    #endif

    // collect votes
    pictureVotes += ballot.picture;
    statueVotes += ballot.statue;
    giftshopVotes += ballot.giftshop;

    Tour tour;

    if ( abs(syncSem.counter()) == group - 1 ) { // check number of waiters note counter would be negative
        tally(); 
        tour.tourkind = destination; tour.groupno = currentGroup;
        #ifdef NOOUTPUT
            #define PRINT(...)
        #else 
            printer.print(id, Voter::States::Complete, tour); // create group
        #endif
    } else {
        #ifdef NOOUTPUT
            #define PRINT(...)
        #else 
            printer.print( id, Voter::States::Block, abs(syncSem.counter()) + 1 ); // waiters including self 
        #endif

        syncSem.P(muxSem); // wait for other voters pass baton

        #ifdef NOOUTPUT
            #define PRINT(...)
        #else 
            printer.print( id, Voter::States::Unblock, abs(syncSem.counter()) ); // waiters not including self since we're unblocked
        #endif
    }

    if (voters < group) {
  muxSem.V(); // release mutex
        _Throw Failed();
    }

    tour.tourkind = destination; tour.groupno = currentGroup;
    if (syncSem.empty()) {
        muxSem.V(); // release mutex when all voters in group leave for tour
    } else {
        syncSem.V(); // allow the waiting voter thread to go
    }
    return tour;
}

void TallyVotes::done() {
  muxSem.P(); // begin critical section
    voters--; // voter finished voting
    if (voters < group && !syncSem.empty()) syncSem.V(); // wake up failing threads
  muxSem.V(); // finished section
}  // done
