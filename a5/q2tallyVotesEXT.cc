#include "q2printer.h"
#include "q2tallyVotes.h"
#include "q2voter.h"
// #include <iostream>

TallyVotes::TallyVotes(
   unsigned int voters,
   unsigned int group,
   Printer & printer
) : voters(voters), group(group), printer(printer) {} // TallyVotes

void TallyVotes::tally() {
   // determine which tour got the most votes
   // note: using >= since tie breaker (g > p > s)
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
   if (voters < group ) _Throw Failed(); // check if there's enough voters

   #ifndef NOOUTPUT
      printer.print(id, Voter::States::Vote, ballot); // print ballot
   #endif

   // collect votes
   pictureVotes += ballot.picture;
   statueVotes += ballot.statue;
   giftshopVotes += ballot.giftshop;
   waitingVoters++;

    if (waitingVoters == group ) { // full tour
      tally(); // count the collected votes
      waitingVoters--;
      Tour tour; tour.tourkind = destination; tour.groupno = currentGroup;
      #ifndef NOOUTPUT
          printer.print(id, Voter::States::Complete, tour); // create group
      #endif
   } else { // wait for others to vote and print blocked and unblock msgs
      #ifndef NOOUTPUT
        printer.print(id, Voter::States::Block, waitingVoters);
      #endif
      _Accept (done) { // check if there's enough voters to make a group
        if ( voters < group ) _Throw Failed(); 
      } _Else {
          _Accept (vote) {
            waitingVoters--;
            #ifndef NOOUTPUT
                printer.print(id, Voter::States::Unblock, waitingVoters);
            #endif
          } // _Accept
      } // _Accept _Else
   }

   Tour tour; tour.tourkind = destination; tour.groupno = currentGroup;
   return tour;
}

void TallyVotes::done() {
  voters--;
}
