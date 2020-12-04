#ifndef TALLYVOTES_H
#define TALLYVOTES_H

_Monitor Printer; // forward declare Printer class

#if defined( EXT )                           // external scheduling monitor solution
// includes for this kind of vote-tallier
_Monitor TallyVotes {
    // private declarations for this kind of vote-tallier
#elif defined( INT )                         // internal scheduling monitor solution
// includes for this kind of vote-tallier
_Monitor TallyVotes {
    uCondition waiting;
    // private declarations for this kind of vote-tallier
#elif defined( INTB )                        // internal scheduling monitor solution with barging
// includes for this kind of vote-tallier
_Monitor TallyVotes {
    // private declarations for this kind of vote-tallier
    uCondition bench;                        // only one condition variable (variable name may be changed)
    void wait();                             // barging version of wait
    void signalAll();                        // unblock all waiting tasks
    unsigned int ticket = 0; // global ticket counter
    unsigned int serving; // the batch of ticket(s) being served, up to `group` voters can be serviced at a time
    unsigned int bargers = 0; // count number of bargers
#elif defined( AUTO )                        // automatic-signal monitor solution
#include "AutomaticSignal.h"
// includes for this kind of vote-tallier
_Monitor TallyVotes {
   AUTOMATIC_SIGNAL;
   bool completed = false; // completed = waitingVoters == group || voters < group
    // private declarations for this kind of vote-tallier
#elif defined( TASK )                        // internal/external scheduling task solution
_Task TallyVotes {
    void main();
    uCondition bench; 
    // private declarations for this kind of vote-tallier
#else
    #error unsupported voter type
#endif
    // common declarations
  public:                                    // common interface
    _Event Failed {};
    struct Ballot { unsigned int picture, statue, giftshop; };
    enum TourKind : char { Picture = 'p', Statue = 's', GiftShop = 'g' };
    struct Tour { TourKind tourkind; unsigned int groupno; };
    TallyVotes( unsigned int voters, unsigned int group, Printer & printer );
    _Mutex Tour vote( unsigned int id, Ballot ballot );
    _Mutex void done();
  private: // common declarations
    void tally() {
        if ( giftshopVotes >= pictureVotes &&  giftshopVotes >= statueVotes ) {
            destination = GiftShop;   
        } else if ( pictureVotes >= statueVotes) {
            destination = Picture;
        } else {
            destination = Statue;
        }
        pictureVotes = 0, statueVotes = 0, giftshopVotes = 0; // reset vote counter
        currentGroup++;
    }; // tally votes when group is formed
    unsigned int waitingVoters = 0; // number of voters waiting to go on tour
    unsigned int voters; // number of voters
    unsigned int group; // group size
    Printer& printer; // printer for output
    unsigned int pictureVotes = 0; // accumulate picture votes 
    unsigned int statueVotes = 0; // accumulate statue votes
    unsigned int giftshopVotes = 0; // accumulate giftshop votes
    unsigned int currentGroup = 0; // track the current group going on tour
    TourKind destination; // what destination is the group going to
#if defined( TASK ) 
    Ballot ballot; // current voter's ballot
    unsigned int id; // current voter id
#endif
};

#endif