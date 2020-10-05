#include <iostream>
using namespace std;
#include "PRNG.h"
#include "q2player.h"

extern PRNG prng;

Player * Player::umpire; // define static member variable

Player::Player(int id, Potato& potato) : id(id), potato(&potato) {}

Player::~Player() {
    partner[1]->partner[0] = partner[0];
    partner[0]->partner[1] = partner[1];
} // ~Player

void Player::start( Player & lp, Player & rp ) { // starter ::main
    partner[0] = &lp; partner[1] = &rp;
    resume(); // activates main
} // Player::start

int Player::getId() {
    return id;
} // Player::getId

void Player::toss() {
    resume(); // Player::main
} // Player::toss

void Player::vote() {
    resume();
} // Player::vote
 
void Player::terminate() {
    resume();
} // Player::Teriminate

void Player::main() { // starter Player::starter
    // first resume starts here 
    suspend();  // suspends after Player coroutine is first started
    Game: for ( ;; ) {
        try {
            _Enable {
                cout << id;
                potato->countdown();
                cout << " -> ";
                partner[prng(1)]->toss();
            } // Enable
        } catch ( Potato::Explode& ) {
            cout << " is eliminated" << endl;
            if ( this == umpire ) { // need to hold election before deleting umpire
                cout << "E " << id;
                try { 
                    _Enable {
                        _Resume Election(partner[1]) _At *partner[1]; // right partner votes
                        partner[1]->vote();
                    } // Enable
                // We've elected the new umpire when the old umpire is resumed again
                } catch (Election & election_won) { 
                    umpire = election_won.player;
                    cout << " : umpire "  << umpire->getId() << endl;
                } // try
            } // if
            _Resume Terminate(*this) _At *umpire;
            umpire->terminate();
        } catch ( Election& election ) { // all none umpire players catch election here
            cout << " -> " << id;
            if ( id > election.player->getId() ) election.player = this; // update winning candidate
            _Resume election _At *partner[1];
            partner[1]->vote();
        } catch ( Terminate& terminate ) {
            delete &terminate.victim;
            potato->reset();
            cout << "U ";
            if ( this == partner[1] ) break Game; // only one player left game is won!
        } // try
    } // Game
    cout << id << " wins the Match!" << endl;
} // Player::main
// end of file