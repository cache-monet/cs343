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
    // private declarations for this kind of vote-tallier
#elif defined( INTB )                        // internal scheduling monitor solution with barging
// includes for this kind of vote-tallier
_Monitor TallyVotes {
    // private declarations for this kind of vote-tallier
    uCondition bench;                        // only one condition variable (variable name may be changed)
    void wait();                             // barging version of wait
    void signalAll();                        // unblock all waiting tasks
#elif defined( AUTO )                        // automatic-signal monitor solution
// includes for this kind of vote-tallier
_Monitor TallyVotes {
    // private declarations for this kind of vote-tallier
#elif defined( TASK )                        // internal/external scheduling task solution
_Task TallyVotes {
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
    Tour vote( unsigned int id, Ballot ballot );
    void done();
    void tally(); // tally votes when group is formed
  private: // common declarations
    unsigned int waitingVoters = 0; // number of voters waiting to go on tour
    unsigned int voters; // number of voters
    unsigned int group; // group size
    Printer& printer; // printer for output
    unsigned int pictureVotes = 0; // accumulate picture votes 
    unsigned int statueVotes = 0; // accumulate statue votes
    unsigned int giftshopVotes = 0; // accumulate giftshop votes
    unsigned int currentGroup = 0; // track the current group going on tour
    TourKind destination; // what destination is the group going to
};

// TODO MOVE THIS INTB.cc when implementing
// void TallyVotes::wait() {
//     bench.wait();                            // wait until signalled
//     while ( rand() % 2 == 0 ) {              // multiple bargers allowed
//         try {
//             _Accept( vote, done ) {          // accept barging callers
//             } _Else {                        // do not wait if no callers
//             } // _Accept
//         } catch( uMutexFailure::RendezvousFailure & ) {}
//     } // while
// }

// void TallyVotes::signalAll() {               // also useful
//     while ( ! bench.empty() ) bench.signal();// drain the condition
// }

#endif