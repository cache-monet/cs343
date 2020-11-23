#include "q2printer.h"
#include "q2tallyVotes.h"
#include "q2voter.h"
#include <iostream>

using namespace std;

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

      for (;;) {
        _Accept(done) {  // check for error if another voter terminated
          if ( voters < group ) {
            #ifndef NOOUTPUT
               printer.print(id, Voter::States::Unblock, waitingVoters-1);
            #endif
             _Throw Failed(); 
          }
        }
        _Else _Accept(vote) { break; }  /// wait until the another voter finish voting voting before progressing
      }

      #ifndef NOOUTPUT
          printer.print(id, Voter::States::Unblock, waitingVoters-1);
      #endif
   }

   waitingVoters--;
   Tour tour; tour.tourkind = destination; tour.groupno = currentGroup;
   return tour;
}

void TallyVotes::done() {
  voters--;
}
