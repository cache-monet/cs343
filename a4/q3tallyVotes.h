#ifndef TALLYVOTES_H
#define TALLYVOTES_H

_Monitor Printer;

#if defined( MC )                    // mutex/condition solution
// includes for this kind of vote-tallier
class TallyVotes {
    // private declarations for this kind of vote-tallier
#elif defined( SEM )                // semaphore solution
#include <uSemaphore.h>
// includes for this kind of vote-tallier
class TallyVotes {
    // private declarations for this kind of vote-tallier
    uSemaphore muxSem; // used for mutual exclusion
    uSemaphore syncSem; // used for synchronizing voters
    void tally(); // tabulate votes; set destination based on election results; increment current group and reset votes
#elif defined( BAR )                // barrier solution
#include <uBarrier.h>
// includes for this kind of vote-tallier
_Cormonitor TallyVotes : public uBarrier {
    // private declarations for this kind of vote-tallier
    void last(); // called by last task to barrier
#else
    #error unsupported voter type
#endif
  public:                            // common interface
    _Event Failed {};
    struct Ballot { unsigned int picture, statue, giftshop; };
    enum TourKind { Picture = 'p', Statue = 's', GiftShop = 'g' };
    struct Tour { TourKind tourkind; unsigned int groupno; };
    TallyVotes( unsigned int voters, unsigned int group, Printer & printer );
    Tour vote( unsigned int id, Ballot ballot );
    void done(
    #if defined( BAR )               // barrier solution
        unsigned int id
    #endif
    );
  private: // common declarations
    unsigned int voters; // number of voters
    unsigned int group; // group size
    Printer& printer; // printer for output
    unsigned int pictureVotes = 0; // accumulate picture votes 
    unsigned int statueVotes = 0; // accumulate statue votes
    unsigned int giftshopVotes = 0; // accumulate giftshop votes
    unsigned int currentGroup = 0; // track the current group going on tour
    TourKind destination;
};
#endif