#include <iostream>
#include <fstream>
#include <string>
#include "q3integerliteral.h"														// defination for the Integer Literal interface
using namespace std;

int main( int argc, char * argv[] ) {
	istream *input = &cin;
  try {                                               // process command-line arguments
    switch ( argc ) {
      case 2:
        input = new ifstream(argv[1]);
      case 1:
        break;
      default: throw 1;
    } // switch
  } catch( ... ) {
    cerr << "Usage: " <<  argv[0] << " [infile-file]" << endl;
    exit( EXIT_FAILURE );
  } // try
  
  *input >> noskipws;
  // cin >> noskipws;
  for ( ;; ) {
    string raw;
    getline(*input, raw);
    if ( input->fail() ) goto finished;

    if (raw == "") {
      cout << "'' : Warning! Blank line." << endl;
      continue;
    }
    int length = raw.length(); int i;
    try {
      _Enable {
        IntegerLiteral integer;
        cout << '\'' << raw << '\'' << " : ";

        for ( i = 0; i < length; ++i ) {
            integer.next(raw[i]);
        } 
        integer.next(IntegerLiteral::EOT);
      } // ENABLE
    } catch ( IntegerLiteral::Match& M ) {
      cout << '\'' << raw.substr(0, i+1) << '\'' << " yes, value " <<  M.value;
    } catch ( IntegerLiteral::Error& E ) {
      cout << '\'' << raw.substr(0, i+1) << '\'' << " no";
    }// try
    if (i < length - 1) {
      cout << " - extraneous characters '";
      for (; i < length-1; i++) cout << raw[i+1];
      cout << "'";
    }
    cout << endl;
  }
  finished:;
  if (input != &cin) delete input;
  return 0;
}
// end of file