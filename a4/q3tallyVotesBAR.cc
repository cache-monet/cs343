#include "q3tallyVotes.h"
#include "q3voter.h"
#include "q3printer.h"

TallyVotes::TallyVotes(
   unsigned int voters,
   unsigned int group,
   Printer & printer
) : uBarrier(group), voters(voters), group(group), printer(printer) {}

void TallyVotes::last() {
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

   #ifdef NOOUTPUT
      #define PRINT(...);
   #else 
      printer.print(id, Voter::States::Vote, ballot); // print ballot
   #endif

   // collect votes
   pictureVotes += ballot.picture;
   statueVotes += ballot.statue;
   giftshopVotes += ballot.giftshop;

   if (uBarrier::waiters() + 1 == uBarrier::total()) { // check if we have enough people to form a group
      uBarrier::block(); // calls last() implicitly
      Tour tour; tour.tourkind = destination; tour.groupno = currentGroup;

      #ifdef NOOUTPUT
         #define PRINT(...);
      #else 
            printer.print(id, Voter::States::Complete, tour); // create group
      #endif

   } else { // wait for others to vote and print blocked and unblock msgs

      #ifdef NOOUTPUT
            #define PRINT(...);
      #else 
            printer.print(id, Voter::States::Block, uBarrier::waiters() + 1); // waiters including self + 1 since we're not waiting yet
      #endif

      uBarrier::block();

      #ifdef NOOUTPUT
            PRINT(id); // waiters not including self (since we're unblocked)
      #else 
            printer.print(id, Voter::States::Unblock, uBarrier::waiters()); // waiters not including self (since we're unblocked)
      #endif
   }

   if ( voters < group ) _Throw Failed(); // check if there is not enough voter at the end
   Tour tour; tour.tourkind = destination; tour.groupno = currentGroup;
   return tour;
}

void TallyVotes::done(unsigned int id) {
   voters--; // voter finished voting
   if (voters < group && waiters() > 0) {
      uBarrier::block(); // wake up failed quorum voters

      #ifdef NOOUTPUT
            PRINT(id); // block in done
      #else 
            printer.print(id, Voter::States::Done); // block in done
      #endif
   }
}  // done
