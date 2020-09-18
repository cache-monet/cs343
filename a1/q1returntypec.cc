#include <iostream>
#include <cstdlib>                                      // access: rand, srand
#include <cstring>                                      // access: strcmp
using namespace std;
#include <unistd.h>                                     // access: getpid

struct ReturnType {
  union {
    double returnval;
    short int ex1code; 
    int ex2code; 
    long int ex3code; 
  };
  enum Tag { NORMAL, EX1, EX2, EX3 };
  Tag tag;
};

int eperiod = 10000;                                    // exception period

ReturnType rtn1 ( double i ) {
    if ( rand() % eperiod == 0 ) {
      ReturnType rtn{}; rtn.tag = ReturnType::Tag::EX1; rtn.ex1code = (short int) rand();
      return rtn;
    }
    ReturnType rtn{}; rtn.tag = ReturnType::Tag::NORMAL; rtn.returnval = i;
    return rtn;
}
ReturnType rtn2( double i ) {
    if ( rand() % eperiod == 0 ) {
      ReturnType rtn{}; rtn.tag = ReturnType::Tag::EX2; rtn.ex2code = rand();
      return rtn;
    }
    ReturnType rtn = rtn1(i);
    if (rtn.tag != ReturnType::Tag::NORMAL) return rtn;
    rtn.returnval += i;
    return rtn;
}
ReturnType rtn3( double i  ) {
    if ( rand() % eperiod == 0 ) {
      ReturnType rtn{}; rtn.tag = ReturnType::Tag::EX3; rtn.ex3code = rand();
      return rtn;
    }
    ReturnType rtn = rtn2( i );
    if (rtn.tag != ReturnType::Tag::NORMAL) {
      return rtn;
    }
    rtn.returnval += i;
    return rtn;
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
        ReturnType rtn = rtn3( i );
        switch ( rtn.tag ) {
          case ReturnType::Tag::NORMAL:
            rv += rtn.returnval; rc += 1;
            break;
          case ReturnType::Tag::EX1:
            ev1 += rtn.ex1code; ec1 += 1;
            break;
          case ReturnType::Tag::EX2:
            ev2 += rtn.ex2code; ec2 += 1;
            break;
          case ReturnType::Tag::EX3:
            ev3 += rtn.ex3code; ec3 += 1;
            break;
        }
    } // for
    cout << "normal result " << rv << " exception results " << ev1 << ' ' << ev2 << ' ' << ev3 << endl;
    cout << "calls "  << rc << " exceptions " << ec1 << ' ' << ec2 << ' ' << ec3 << endl;
}
