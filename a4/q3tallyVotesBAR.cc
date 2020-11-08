#include <iostream>
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

   printer.print(id, Voter::States::Vote, ballot); // print ballot
   // collect votes
   pictureVotes += ballot.picture;
   statueVotes += ballot.statue;
   giftshopVotes += ballot.giftshop;

   if (uBarrier::waiters() + 1 == uBarrier::total()) { // check if we have enough people to form a group
      uBarrier::block(); // calls last() implicitly
      printer.print(id, Voter::States::Complete, Tour(destination, currentGroup)); // create group
   } else { // wait for others to vote and print blocked and unblock msgs
      printer.print(id, Voter::States::Block, uBarrier::waiters() + 1); // waiters including self + 1 since we're not waiting yet
      uBarrier::block();
      printer.print(id, Voter::States::Unblock, uBarrier::waiters()); // waiters not including self (since we're unblocked)
   }

   if ( voters < group ) _Throw Failed(); // check if there is not enough voter at the end
   return Tour(destination, currentGroup);
}

void TallyVotes::done(unsigned int id) {
   voters--; // voter finished voting
   if (voters < group && waiters() > 0) {
      uBarrier::block(); // wake up failed quorum voters
      printer.print(id, Voter::States::Done); // block in done
   }
   printer.print(id, Voter::States::Terminated);
}  // done
