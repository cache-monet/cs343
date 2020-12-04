#include "q2printer.h"
#include "q2tallyVotes.h"
#include "q2voter.h"
#include <iostream>

TallyVotes::TallyVotes(
   unsigned int voters,
   unsigned int group,
   Printer & printer
) : voters(voters), group(group), printer(printer) {} // TallyVotes

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
   if (voters < group ) _Throw Failed(); // check if there's enough voters

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
   }
   else
   { // wait for others to vote and print blocked and unblock msgs
      #ifndef NOOUTPUT
        printer.print(id, Voter::States::Block, waitingVoters);
      #endif

      waiting.wait();

      #ifndef NOOUTPUT
          printer.print(id, Voter::States::Unblock, waitingVoters-1);
      #endif

      if (voters < group ) {
         waitingVoters--;
         _Throw Failed(); // check if there's enough voters after waiting up
      }
   }

   waitingVoters--;
   Tour tour; tour.tourkind = destination; tour.groupno = currentGroup;
   waiting.signal();
   return tour;
}

void TallyVotes::done() {
  voters--;
  if (voters < group) waiting.signal(); // wake up failed voters
}
