#include <iostream>
#include <cstdlib>                                      // access: rand, srand
#include <cstring>                                      // access: strcmp
#include <variant>
using namespace std;
#include <unistd.h>                                     // access: getpid

struct Ex1 { short int code; };
struct Ex2 { int code; };
struct Ex3 { long int code; };

int eperiod = 10000;                                    // exception period

variant<double, Ex1> rtn1( double i ) {
    if ( rand() % eperiod == 0 ) return Ex1{(short int) rand()};
    return i;
}

variant<double, Ex1, Ex2> rtn2( double i ) {
    if ( rand() % eperiod == 0 ) return Ex2{ rand() };

    variant<double, Ex1> r1 = rtn1( i );
    if ( holds_alternative<double>(r1) ) return get<double>(r1) + i;
    return get<Ex1>(r1);
}
variant <double, Ex1, Ex2, Ex3> rtn3( double i  ) {
    if ( rand() % eperiod == 0 ) return Ex3{ (long int) rand() };

    variant<double, Ex1, Ex2> r2 = rtn2( i );
    if ( holds_alternative<double>(r2) ) return get<double>(r2) + i;
    if ( holds_alternative<Ex1>(r2) ) return get<Ex1>(r2);
    return get<Ex2>(r2);
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

    double rv = 0.0;
    int ev1 = 0, ev2 = 0, ev3 = 0;
    int rc = 0, ec1 = 0, ec2 = 0, ec3 = 0;

    for ( int i = 0; i < times; i += 1 ) {
        variant <double, Ex1, Ex2, Ex3> r3 = rtn3 ( i );
        if ( holds_alternative<double>(r3) ) {
            rv += get<double>(r3); rc += 1;
        } else if ( holds_alternative<Ex1>(r3) ) {
            ev1 += get<Ex1>(r3).code; ec1 += 1;
        } else if ( holds_alternative<Ex2>(r3) ) {
            ev2 += get<Ex2>(r3).code; ec2 += 1;
        } else {
            ev3 += get<Ex3>(r3).code; ec3 += 1;
        }
    } // for
    cout << "normal result " << rv << " exception results " << ev1 << ' ' << ev2 << ' ' << ev3 << endl;
    cout << "calls "  << rc << " exceptions " << ec1 << ' ' << ec2 << ' ' << ec3 << endl;
}
