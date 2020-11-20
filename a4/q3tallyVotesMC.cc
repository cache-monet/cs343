#include "q3printer.h"
#include "q3tallyVotes.h"
#include "q3voter.h"
#include <iostream>

TallyVotes::TallyVotes(
   unsigned int voters,
   unsigned int group,
   Printer & printer
) : voters(voters), group(group), printer(printer) {}

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

  voteLock.acquire();


    if (barge) {
        bargers++; // manange barger
        #ifdef NOOUTPUT
            #define PRINT(...)
        #else 
            printer.print( id, Voter::States::Barging, bargers, currentGroup); 
        #endif
        bargeLock.wait(voteLock);
        bargers--;
        barge = !bargeLock.empty();
    }

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

    if (waitingVoters == group - 1) { // we're completing the group
        tally(); 
        tour.tourkind = destination; tour.groupno = currentGroup;

    #ifdef NOOUTPUT
        #define PRINT(...)
    #else 
        printer.print(id, Voter::States::Complete, tour); // create group
    #endif
    } else {
        waitingVoters++; // add ourselves to the waiting queue
        #ifdef NOOUTPUT
            #define PRINT(...)
        #else 
            printer.print( id, Voter::States::Block, waitingVoters ); // waiters including self 
        #endif

        bargeLock.signal(); // signal a barger to join group
        groupLock.wait(voteLock); // release voteLock before going to sleep
        waitingVoters--; // remove ourself from waiting

        #ifdef NOOUTPUT
            #define PRINT(...)
        #else 
            printer.print( id, Voter::States::Unblock, waitingVoters ); // waiters not including self
        #endif
    }

    tour.tourkind = destination; tour.groupno = currentGroup;

    barge = !( groupLock.empty() && bargeLock.empty() ); // we are entering the going on tour section
    // any thread trying to enter before all voters in groupLock exits are barging
    // barge = !groupLock.empty() || !bargeLock.empty();

    if (voters < group ) {
  voteLock.release();
        _Throw Failed(); // check if there's enough remaning voters to create a group
    }

    groupLock.empty() ? bargeLock.signal() : groupLock.signal();
    // if no one's in groupLock signal barge; else groupLock has priority

  voteLock.release();

    return tour;
}

void TallyVotes::done() { // a group is formed
  voteLock.acquire();
    voters--;
    if ( voters < group && !(groupLock.empty() && bargeLock.empty()) ) { // not enough voters
        // wake up failing threads
        groupLock.empty() ? bargeLock.signal() : groupLock.signal();
    }
  voteLock.release();
}