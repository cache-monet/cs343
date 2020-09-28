#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>                                     // access: getpid

// handle exceptions reading cmd line arguments
typedef enum { NOERROR, EXCEPTIONAL } CmdTag ;

// handle exceptions for rtn functions
// if no exception has occured use returnval and NORMAL
typedef enum { NORMAL, EX1, EX2, EX3 } RtnTag ;
typedef struct {
  RtnTag tag;
  union {
    double returnval;
    short int ex1code; 
    int ex2code; 
    long int ex3code; 
  };
} RtnType;


int eperiod = 10000;                                    // exception period

RtnType rtn1 ( double i ) {
    if ( rand() % eperiod == 0 ) {
      RtnType rtn = {.tag = EX1, .ex1code = (short int) rand() };
      return rtn;
    }
    RtnType rtn = {.tag = NORMAL, .returnval = i };
    return rtn;
}
RtnType rtn2( double i ) {
    if ( rand() % eperiod == 0 ) {
      RtnType rtn = {.tag = EX2, .ex2code = rand() };
      return rtn;
    }
    RtnType rtn = rtn1(i);
    if (rtn.tag != NORMAL) return rtn;
    rtn.returnval += i;
    return rtn;
}
RtnType rtn3( double i  ) {
    if ( rand() % eperiod == 0 ) {
      RtnType rtn ={ .tag = EX3, .ex3code = rand() } ;
      return rtn;
    }
    RtnType rtn = rtn2( i );
    if (rtn.tag != NORMAL) {
      return rtn;
    }
    rtn.returnval += i;
    return rtn;
}
int main( int argc, char * argv[] ) {
    int times = 100000000, seed = getpid();             // default values
    CmdTag status = NOERROR; // set cmd status to be the normal at first
    switch ( argc ) {
      case 4: if ( strcmp( argv[3], "d" ) != 0 ) {  // default ?
            seed = atoi( argv[3] ); 
            if ( seed <= 0 ) {
              // update status when there's an invalid input and exit switch statement
              status = EXCEPTIONAL;
              break;
            }
        } // if
      case 3: if ( strcmp( argv[2], "d" ) != 0 ) {  // default ?
            eperiod = atoi( argv[2] );
            if ( eperiod <= 0 ) {
              status = EXCEPTIONAL;
              break;
            }
        } // if
      case 2: if ( strcmp( argv[1], "d" ) != 0 ) {  // default ?
            times = atoi( argv[1] );
            if ( times <= 0 ) {
              status = EXCEPTIONAL;
              break;
            }
        } // if
      case 1: break;                                // use all defaults
      default: status = EXCEPTIONAL;
    } if (status != NOERROR ) {
        printf("Usage: %s [times > 0 | d [eperiod > 0 | d [ seed > 0 ] ] ]\n", argv[0]);
        exit( EXIT_FAILURE );
    };
    srand( seed );

    double rv = 0.0;
    int ev1 = 0, ev2 = 0, ev3 = 0;
    int rc = 0, ec1 = 0, ec2 = 0, ec3 = 0;

    for ( int i = 0; i < times; i += 1 ) {
        RtnType rtn = rtn3( i );
        switch ( rtn.tag ) {
          case NORMAL:
            rv += rtn.returnval; rc += 1;
            break;
          case EX1:
            ev1 += rtn.ex1code; ec1 += 1;
            break;
          case EX2:
            ev2 += rtn.ex2code; ec2 += 1;
            break;
          case EX3:
            ev3 += rtn.ex3code; ec3 += 1;
            break;
        }
    } // for
    printf("normal result %g exception results %d %d %d\n", rv, ev1, ev2, ev3);
    printf("calls %d exceptions %d %d %d\n", rc, ec1, ec2, ec3);
}
