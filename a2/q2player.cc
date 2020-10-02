#include <iostream>
using namespace std;
#include "PRNG.h"
#include "q2player.h"

extern PRNG prng;

Player * Player::umpire; // define static member variable

Player::Player(int id, Potato& potato) : id(id), potato(&potato) {}

void Player::start( Player & lp, Player & rp ) {
    partner[0] = &lp; partner[1] = &rp;
    resume();
}

int Player::getId() {
    return id;
}

void Player::toss() {
    resume();
}

void Player::main() {
    bool first_toss = true;
    suspend(); // suspend for when players are created
    PlayerMain: for ( ;; ) {
        try {
            _Enable {
                if (this == Player::umpire && first_toss ) {
                    cout << "U " << id;
                    first_toss = false;
                } else {
                    cout << " -> " << id;
                }
                if ( this == partner[1] ) {
                    break PlayerMain; // only one player left game is won!
                }
                potato->countdown();
                partner[prng(1)]->toss();
            } // Enable
        } catch ( Potato::Explode& ) {
            cout << " is eliminated" << endl;
            if ( this == umpire ) { // hold election
                cout << "E " << id;
                try { // start election
                    _Enable {
                        _Resume Election(partner[1]) _At *partner[1];
                        partner[1]->vote();
                    } // Enable
                } catch (Election & election ) { // We elected the new umpire
                    cout << " : umpire "  << election.player->getId() << endl;
                    Player::umpire = election.player;
                } // try
            } // if
            _Resume Terminate(*this) _At *umpire;
            umpire->terminate();
        } catch ( Election& election ) {
            cout << " -> " << id; 
            if ( id > election.player->getId() ) election.player = this; // update winning candidate
            _Resume election _At *partner[1];
            partner[1]->vote();
        } catch ( Terminate& terminate ) {
            Player& victim = terminate.victim;
            victim.partner[1]->partner[0] = victim.partner[0];
            victim.partner[0]->partner[1] = victim.partner[1];
            first_toss = true;
            potato->reset();
        } // try
    } // for
    cout << " wins the Match!" << endl;
} // Player::main

void Player::vote() {
    resume();
}
 
void Player::terminate() {
    resume();
} // Player::Teriminate