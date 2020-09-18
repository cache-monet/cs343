#include <iostream>
#include <cstdlib>                                      // access: rand, srand
#include <cstring>                                      // access: strcmp
using namespace std;
#include <unistd.h>                                     // access: getpid

int status;

short int rtn1status;
int rtn2status;
long int rtn3status;

int eperiod = 10000;                                    // exception period

double rtn1( double i ) {
    if ( rand() % eperiod == 0 ) {
        rtn1status = (short int) rand();
        return 0.0;
    }
    return i;
}
double rtn2( double i  ) {
    if ( rand() % eperiod == 0 ) {
        rtn2status = rand();
        return 0.0;
    }
    return rtn1( i ) + i;
}
double rtn3( double i  ) {
    if ( rand() % eperiod == 0 ) {
        rtn3status = (long int) rand();
        return 0.0;
    }
    return rtn2( i ) + i;
}
int main( int argc, char * argv[] ) {
    int times = 100000000, seed = getpid();             // default values
    try {
        switch ( argc ) {
          case 4: if ( strcmp( argv[3], "d" ) != 0 ) {  // default ?
                seed = stoi( argv[3] ); if ( seed <= 0 ) throw 1;
            } // if
          case 3: if ( strcmp( argv[2], "d" ) != 0 ) {  // default ?
                eperiod = stoi( argv[2] ); if ( eperiod <= 0 ) throw 1;
            } // if
          case 2: if ( strcmp( argv[1], "d" ) != 0 ) {  // default ?
                times = stoi( argv[1] ); if ( times <= 0 ) throw 1;
            } // if
          case 1: break;                                // use all defaults
          default: throw 1;
        } // switch
    } catch( ... ) {
        cerr << "Usage: " << argv[0] << " [ times > 0 | d [ eperiod > 0 | d [ seed > 0 ] ] ]" << endl;
        exit( EXIT_FAILURE );
    } // try
    srand( seed );

    double rv = 0.0; int ev1 = 0, ev2 = 0, ev3 = 0;
    int rc = 0, ec1 = 0, ec2 = 0, ec3 = 0;

    for ( int i = 0; i < times; i += 1 ) {
        int val = rtn3( i );
        // val can be any value if an exception has a occured;
        // so only use it after checking no error occured
        if (rtn1status != 0) {
            ev1 += rtn1status; rtn1status = 0; ec1 += 1;
        } else if (rtn2status != 0) {
            ev2 += rtn2status; rtn2status = 0; ec2 += 1; 
        } else if (rtn3status != 0) {
            ev3 += rtn3status; rtn3status = 0;
            ec3 += 1;
        } else {
            rv += val; rc += 1;
        }
    } // for
    cout << "normal result " << rv << " exception results " << ev1 << ' ' << ev2 << ' ' << ev3 << endl;
    cout << "calls "  << rc << " exceptions " << ec1 << ' ' << ec2 << ' ' << ec3 << endl;
}
