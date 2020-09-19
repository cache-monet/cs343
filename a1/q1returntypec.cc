#include <iostream>
#include <cstdlib>                                      // access: rand, srand
#include <cstring>                                      // access: strcmp
using namespace std;
#include <unistd.h>                                     // access: getpid

// handle exceptions for rtn functions
// if no exception has occured use returnval and NORMAL
struct RtnType {
  union {
    double returnval;
    short int ex1code; 
    int ex2code; 
    long int ex3code; 
  };
  enum Tag { NORMAL, EX1, EX2, EX3 };
  Tag tag;
};

// handle exceptions reading cmd line arguments
struct CmdStatus {
  enum Tag {NORMAL, EXCEPTIONAL};
  Tag tag; 
};

int eperiod = 10000;                                    // exception period

RtnType rtn1 ( double i ) {
    if ( rand() % eperiod == 0 ) {
      RtnType rtn{}; rtn.tag = RtnType::Tag::EX1; rtn.ex1code = (short int) rand();
      return rtn;
    }
    RtnType rtn{}; rtn.tag = RtnType::Tag::NORMAL; rtn.returnval = i;
    return rtn;
}
RtnType rtn2( double i ) {
    if ( rand() % eperiod == 0 ) {
      RtnType rtn{}; rtn.tag = RtnType::Tag::EX2; rtn.ex2code = rand();
      return rtn;
    }
    RtnType rtn = rtn1(i);
    if (rtn.tag != RtnType::Tag::NORMAL) return rtn;
    rtn.returnval += i;
    return rtn;
}
RtnType rtn3( double i  ) {
    if ( rand() % eperiod == 0 ) {
      RtnType rtn{}; rtn.tag = RtnType::Tag::EX3; rtn.ex3code = rand();
      return rtn;
    }
    RtnType rtn = rtn2( i );
    if (rtn.tag != RtnType::Tag::NORMAL) {
      return rtn;
    }
    rtn.returnval += i;
    return rtn;
}
int main( int argc, char * argv[] ) {
    int times = 100000000, seed = getpid();             // default values
    CmdStatus status{ CmdStatus::Tag::NORMAL }; // set cmd status to be the normal at first
    switch ( argc ) {
      case 4: if ( strcmp( argv[3], "d" ) != 0 ) {  // default ?
            seed = stoi( argv[3] ); 
            if ( seed <= 0 ) {
              // update status when there's an invalid input and exit switch statement
              status = CmdStatus{ CmdStatus::Tag::EXCEPTIONAL };
              break;
            }
        } // if
      case 3: if ( strcmp( argv[2], "d" ) != 0 ) {  // default ?
            eperiod = stoi( argv[2] );
            if ( eperiod <= 0 ) {
              status = CmdStatus{ CmdStatus::Tag::EXCEPTIONAL };
              break;
            }
        } // if
      case 2: if ( strcmp( argv[1], "d" ) != 0 ) {  // default ?
            times = stoi( argv[1] );
            if ( times <= 0 ) {
              status = CmdStatus{ CmdStatus::Tag::EXCEPTIONAL };
              break;
            }
        } // if
      case 1: break;                                // use all defaults
      default: status = CmdStatus{ CmdStatus::Tag::EXCEPTIONAL };
    } if (status.tag != CmdStatus::Tag::NORMAL) {
        cerr << "Usage: " << argv[0] << " [ times > 0 | d [ eperiod > 0 | d [ seed > 0 ] ] ]" << endl;
        exit( EXIT_FAILURE );
    };
    srand( seed );

    double rv = 0.0;
    int ev1 = 0, ev2 = 0, ev3 = 0;
    int rc = 0, ec1 = 0, ec2 = 0, ec3 = 0;

    for ( int i = 0; i < times; i += 1 ) {
        RtnType rtn = rtn3( i );
        switch ( rtn.tag ) {
          case RtnType::Tag::NORMAL:
            rv += rtn.returnval; rc += 1;
            break;
          case RtnType::Tag::EX1:
            ev1 += rtn.ex1code; ec1 += 1;
            break;
          case RtnType::Tag::EX2:
            ev2 += rtn.ex2code; ec2 += 1;
            break;
          case RtnType::Tag::EX3:
            ev3 += rtn.ex3code; ec3 += 1;
            break;
        }
    } // for
    cout << "normal result " << rv << " exception results " << ev1 << ' ' << ev2 << ' ' << ev3 << endl;
    cout << "calls "  << rc << " exceptions " << ec1 << ' ' << ec2 << ' ' << ec3 << endl;
}
