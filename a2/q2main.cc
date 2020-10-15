// Purpose: copy file
#include <iostream>
using namespace std;					// direct access to std
#include <vector>
#include <cstring>                                      // access: strcmp
#include <cstdlib>					// exit
#include <unistd.h>                                     // access: getpid
#include "PRNG.h"                   // random number generator
#include "q2potato.h"
#include "q2player.h"

PRNG prng; // set global random nubmer generator

int main( int argc, char * argv[] ) {
    const int DefaultGames = 5;
    const int DefaultSeed = getpid();		// global defaults
    // MUST BE INT (NOT UNSIGNED) TO CORRECTLY TEST FOR NEGATIVE VALUES

    int gamesToPlay = DefaultGames, seed = DefaultSeed;
    prng.seed(seed); // set prng seed to devault value
    int players; bool players_specified;
    try {
        switch ( argc ) {
          case 4: if ( strcmp( argv[3], "d" ) != 0 ) {  // default ?
                seed = stoi( argv[3] ); if ( seed <= 0 ) throw 1;
                prng.seed(seed); // update seed if it's specified
            } // if
          case 3: 
              if  ( strcmp( argv[2], "d" ) != 0 ) {
                players = stoi( argv[2] );
                players_specified = true;
              }
              if (players < 2 ) throw 1;
          case 2: if ( strcmp( argv[1], "d" ) != 0 ) {  // default ?
                gamesToPlay = stoi( argv[1] ); if ( gamesToPlay < 0 ) throw 1;
            } // if
          case 1: break;                                // use all defaults
          default: throw 1;
        } // switch
    } catch( ... ) {
	    cerr << "Usage: " << argv[0]
	        << " [ games (>=0) | 'd' (default 5)"
	        << " [ players (>=2) | 'd' (random 2-10)"
	        << " [ seed (>0) | 'd' (random) ] ] ]" << endl;
	    exit( EXIT_FAILURE );				// TERMINATE
    } // try
    for ( ; gamesToPlay != 0 ; gamesToPlay--) {
        // if number of players isn't specified randomly generate players for each game
        if (!players_specified) players = prng(2, 10);
        cout << players << " players in the game" << endl;

        Potato potato; // print toss message

        // Create players
        vector<Player*> player_queue;
        for (int i= 0; i< players; i++) player_queue.push_back(new Player(i, potato));
        Player::umpire = player_queue[0]; // Set umpire 

        swap(player_queue[0], player_queue[prng(0, players-1)]); // swap places btwn the first player and a random one

        CreateCycle: for ( int i = 0; i < players; i++) { 
            int left = (i + players - 1) % players; // left of zero is be the "last" player
            int right = (i + 1) % players; // right of last player is zero
            player_queue[i]->start(*player_queue[left], *player_queue[right]);
        } // CreateCycle

        // start the game
        cout << "U "; Player::umpire->toss();
        // Clean up: only umpire is left at the end of game
        delete Player::umpire;

        // print out 2 newlines between each game
        if (gamesToPlay != 1)  {
            cout << endl << endl;
        }
    } // for
} // main
// end of file