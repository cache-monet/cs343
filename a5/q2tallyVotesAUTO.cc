#include "q2printer.h"
#include "q2tallyVotes.h"
#include "q2voter.h"

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

   if ( waitingVoters == group )
   { // full tour
      tally(); // count the collected votes
      Tour tour; tour.tourkind = destination; tour.groupno = currentGroup;
      #ifndef NOOUTPUT
          printer.print(id, Voter::States::Complete, tour); // create group
      #endif
      completed = true;
   }
   else
   { // wait for others to vote and print blocked and unblock msgs
      #ifndef NOOUTPUT
        WAITUNTIL(
          completed,
          printer.print(id, Voter::States::Block, waitingVoters),
          printer.print(id, Voter::States::Unblock, waitingVoters-1)
        ); // wake up when we fill a group or quorom failure
      #else 
        WAITUNTIL( completed , , ); // wake up when we fill a group or quorom failure
      #endif

      if ( voters < group ) {
        waitingVoters--;
        _Throw Failed(); 
      }
   }

   waitingVoters--;
   completed = !(waitingVoters == 0); // close the gate when all voters departed for tour
   Tour tour; tour.tourkind = destination; tour.groupno = currentGroup;

   EXIT();
   return tour;
}

void TallyVotes::done() {
  voters--;
  completed = (voters < group); // wake everyone if there's not enough people left
  EXIT();
}
