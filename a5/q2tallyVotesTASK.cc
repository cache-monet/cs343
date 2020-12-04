#include "q2printer.h"
#include "q2tallyVotes.h"
#include "q2voter.h"
#include <iostream>

TallyVotes::TallyVotes(
   unsigned int voters,
   unsigned int group,
   Printer & printer
) : voters(voters), group(group), printer(printer) {} // TallyVotes

void TallyVotes::main() {
    for ( ;; )  {
        try {
            _Accept(~TallyVotes) { // stop task upon  termination
                break;
            } or _Accept(vote) {
                // collect votes
                pictureVotes += ballot.picture;
                statueVotes += ballot.statue;
                giftshopVotes += ballot.giftshop;
                waitingVoters++;

                #ifndef NOOUTPUT
                    printer.print(id, Voter::States::Vote, ballot); // print ballot
                #endif

                if (waitingVoters == group) { // wake up all failures
                    tally();
                    Tour tour; tour.tourkind = destination; tour.groupno = currentGroup;
                    #ifndef NOOUTPUT
                        printer.print(id, Voter::States::Complete, tour); // create group
                    #endif
                    while( !bench.empty() ) {
                        #ifndef NOOUTPUT
                            printer.print(bench.front(), Voter::States::Unblock, waitingVoters-1);
                        #endif
                        bench.signalBlock(); // signalblock b/c we're a task
                    }
                } else {
                    #ifndef NOOUTPUT
                        printer.print(id, Voter::States::Block, waitingVoters);
                    #endif
                }
            } or _Accept(done) {
                #ifndef NOOUTPUT
                    printer.print(id, Voter::States::Done); // woke up to check for done
                #endif
                if (voters < group) { // wake up all failures
                    while( !bench.empty() ) {
                        #ifndef NOOUTPUT
                            printer.print(bench.front(), Voter::States::Unblock, waitingVoters-1);
                        #endif
                        bench.signalBlock(); // signalblock b/c we're a task
                    }
                }
            }
        } catch (uMutexFailure::RendezvousFailure& ) { // another voter threw; wake everyone else up to throw
            while( !bench.empty() ) {
                #ifndef NOOUTPUT
                    printer.print(bench.front(), Voter::States::Unblock, waitingVoters-1);
                #endif
                bench.signalBlock(); // signalblock b/c we're a task
            }
        } // try
    } // for
}


TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
    TallyVotes::id = id;
    TallyVotes::ballot = ballot;
    if (voters < group ) _Throw Failed(); // check if there's enough voters

    bench.wait(id); // will be woken up
    waitingVoters--;
    if (voters < group ) _Throw Failed(); // check if there's enough voters
    Tour tour; tour.tourkind = destination; tour.groupno = currentGroup;
    return tour;
}

void TallyVotes::done() {
  voters--;
}
