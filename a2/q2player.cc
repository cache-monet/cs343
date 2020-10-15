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

void Player::turn_actions() {
    cout << id;
    potato->countdown();
    // only runs if potato doesn't explode
    cout << " -> ";
    partner[prng(1)]->toss();
} // Player::turn_actions

void Player::election() {
    cout << "E " << id;
    try { 
        _Enable {
            _Resume Election(partner[1]) _At *partner[1]; // right partner votes
            partner[1]->vote();
        } // Enable
    // Every player has voted when the old umpire is resumed again
    } catch (Election & finished) { 
        // inaugurate the new umpire before deleting the old
        umpire = finished.player;
        cout << " : umpire "  << umpire->getId() << endl; 
    } // try
} // Player::election

void Player::main() { // starter Player::starter
    // first resume starts here 
    suspend();  // suspends after Player coroutine is first started
    // toss exploding potatoes until only one player is left
    Game: for ( ;; ) {
        try {
            _Enable {
                turn_actions();
            } // Enable
        } catch ( Potato::Explode& ) {
            cout << " is eliminated" << endl;
            // need to hold election and inaugurate new umpire before deleting old umpire
            if ( this == umpire ) election();
            // transfer control to umpire to delete victim
            _Resume Terminate(*this) _At *umpire;
            umpire->terminate();
        } // catch
        // all non-umpire players catch election here
        catch ( Election& election ) { 
            cout << " -> " << id;
            if ( id > election.player->getId() ) election.player = this; // update winning candidate
            _Resume election _At *partner[1];
            partner[1]->vote();
        } // catch
        // deletes loser and resets potato counter; should only be caught by umpire
        catch ( Terminate& terminate ) { 
            delete &terminate.victim;
            potato->reset();
            cout << "U ";
            if ( this == partner[1] ) break Game; // only one player left game is won!
        } // end of try-catch block
    } // Game
    cout << id << " wins the Match!" << endl; // print out winner id
} // Player::main
// end of file