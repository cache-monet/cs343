#include <iostream>
#include "MPRNG.h"
#include "q3printer.h"
#include "q3voter.h"
#include "q3tallyVotes.h"

using namespace std;

MPRNG mprng; // globally defined mprng

int main(int argc, char* argv[]) {
    int voters = 6, group = 3, votes = 1;
    int seed = getpid(), processors = 1;
    try {
        switch (argc) {
            case 6: if ( strcmp( argv[5], "d" ) != 0 ) {  // default ?
                processors = stoi(argv[5]); if (processors <= 0) throw 1;
            }
            case 5: if ( strcmp( argv[4], "d" ) != 0 ) {  // default ?
                seed = stoi(argv[4]); if (seed <= 0) throw 1;
            }
            case 4: if ( strcmp( argv[3], "d" ) != 0 ) {  // default ?
               votes = stoi(argv[3]); if (votes <= 0) throw 1;
            }
            case 3: if ( strcmp( argv[2], "d" ) != 0 ) {  // default ?
                group = stoi(argv[2]); if (group <= 0) throw 1;
            }
            case 2: if ( strcmp( argv[1], "d" ) != 0 ) {  // default ?
                voters = stoi(argv[1]); if (voters <= 0) throw 1;
            }
            case 1: break;
            default: throw 1;
        }
    } catch (...) {
        cout << "Usage: " << argv[0]
            << " [ voters (> 0) | 'd' (default 6) [ group (> 0) | 'd' (default 3) [ votes (> 0) | 'd' (default 1) [ seed (> 0), | 'd' (default random) [ processors (> 0), | 'd' (default 1) ] ] ] ] ]"
            << endl;
        exit (1);
    } // try

    if (processors != 0) uProcessor p[processors - 1] __attribute__(( unused )); // add multi-processor if that's there's more than one

    mprng.set_seed( seed );

    Printer printer(voters);

    TallyVotes tally(voters, group, printer);

    Voter* voterList[voters];

    for (int i = 0; i < voters; i++) voterList[i] = new Voter(i, votes, tally, printer);

    for (int i = 0; i < voters; i++) delete voterList[i];

    return 0;
}