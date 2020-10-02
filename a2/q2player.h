/* Interface definition for the Player co-routine */
#ifndef __PLAYER_H__
#define __PLAYER_H__
#include  "q2potato.h"

_Coroutine Player {
    _Event Terminate {
      public:
        Player & victim;                    // delete player
        Terminate( Player & victim ) : victim( victim ) {}
    };
    _Event Election {
      public:
        Player * player;                    // highest player id seen so far
        Election( Player * player ) : player( player ) {}
    };
    int id;
    Player * partner[2];                    // left and right player [0] for left [1] for right
    Potato * potato;                        // avoid reference in members
    // YOU ADD MEMBERS HERE
    void main();
    void vote();                            // resume partner to vote
    void terminate();                       // resume umpire
  public:
    static Player * umpire;                 // current umpire

    Player( int id, Potato & potato );
    void start( Player & lp, Player & rp ); // supply partners
    int getId();                            // player id
    void toss();                            // tossed potato
};
// end of file
#endif