#include <iostream>
#include <cstdlib>                                      // access: rand, srand
#include <cstring>                                      // access: strcmp
using namespace std;
#include <unistd.h>                                     // access: getpid

#ifdef NOOUTPUT
#define PRT( stmt )
#else
#define PRT( stmt ) stmt
#endif // NOOUTPUT

struct E {};                                            // exception type
long int eperiod = 100, excepts = 0, calls = 0, dtors = 0; // counters
PRT( struct T { ~T() { dtors += 1; } }; )

long int Ackermann( long int m, long int n ) {
    calls += 1;
    if ( m == 0 ) {
        if ( rand() % eperiod <= 2 ) { PRT( T t; ) excepts += 1; throw E(); } // replace
        return n + 1;
    } else if ( n == 0 ) {
        try { return Ackermann( m - 1, 1 );             // replace
        } catch( E ) {                                  // replace
            PRT( cout << "E1 " << m << " " << n );
            if ( rand() % eperiod <= 1 ) { PRT( T t; ) excepts += 1; throw E(); }
        } // try
    	PRT( cout << " E1X " << m << " " << n << endl );
    } else {
        try { return Ackermann( m - 1, Ackermann( m, n - 1 ) ); // replace
        } catch( E ) { // replace
            PRT( cout << "E2 " << m << " " << n );
            if ( rand() % eperiod == 0 ) { PRT( T t; ) excepts += 1; throw E(); }
        } // try
		PRT( cout << endl << " E2X " << m << " " << n << endl );
    } // if
    return 0;                                           // recover by returning 0
}
int main( int argc, char * argv[] ) {
    long int m = 4, n = 6, seed = getpid();             // default values
    try {                                               // process command-line arguments
        switch ( argc ) {
          case 5: if ( strcmp( argv[4], "d" ) != 0 ) {  // default ?
                eperiod = stoi( argv[4] ); if ( eperiod <= 0 ) throw 1;
            } // if
          case 4: if ( strcmp( argv[3], "d" ) != 0 ) {  // default ?
                seed = stoi( argv[3] ); if ( seed <= 0 ) throw 1;
            } // if
          case 3: if ( strcmp( argv[2], "d" ) != 0 ) {  // default ?
                n = stoi( argv[2] ); if ( n < 0 ) throw 1;
            } // if
          case 2: if ( strcmp( argv[1], "d" ) != 0 ) {  // default ?
                m = stoi( argv[1] ); if ( m < 0 ) throw 1;
            } // if
          case 1: break;                                // use all defaults
          default: throw 1;
        } // switch
    } catch( ... ) {
        cerr << "Usage: " << argv[0] << " [ m (>= 0) | d [ n (>= 0) | d"
             " [ seed (> 0) | d [ eperiod (> 0) | d ] ] ] ]" << endl;
        exit( EXIT_FAILURE );
    } // try
    srand( seed );                                      // seed random number
    try {                                               // replace
        PRT( cout << "Arguments " << m << " " << n << " " << seed << " " << eperiod << endl );
        long int val = Ackermann( m, n );
        PRT( cout << "Ackermann " << val << endl );
    } catch( E ) {                                      // replace
        PRT( cout << "E3" << endl );
    } // try
    cout << "calls " << calls << ' ' << " exceptions " << excepts << " destructors " << dtors << endl;
}
// end of file