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

      // infinite loop for accepting both vote and done
      // exit immediately upon _Accept(vote)
      // _Accept(done) checks for failure condition (voters < group) throws if true else continues to
      // spin
      for ( ;; ) {
        try { // handle quorum failure in _Accept(vote)
          _Accept(vote) {
            #ifndef NOOUTPUT
                printer.print(id, Voter::States::Unblock, waitingVoters-1);
            #endif
            break;
          } or _Accept(done) { // throw error if voter < group otherwise continue spinning
            #ifndef NOOUTPUT
              printer.print(id, Voter::States::Done); // woke up to check for done
            #endif
            if (voters < group) {
              waitingVoters--;
              #ifndef NOOUTPUT
                  printer.print(id, Voter::States::Unblock, waitingVoters);
              #endif

              _Throw Failed();
            }
          }
        } catch (uMutexFailure::RendezvousFailure& ) {
          // another voter threw an error (b/c voter < group) we should throw too
          waitingVoters--;
          #ifndef NOOUTPUT
              printer.print(id, Voter::States::Unblock, waitingVoters);
          #endif

          _Throw Failed();
        } // try
      } // for
   }

   waitingVoters--;
   Tour tour; tour.tourkind = destination; tour.groupno = currentGroup;
   return tour;
}

void TallyVotes::done() {
  voters--;
}
