#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;					// direct access to std
#include <cstdlib>					// exit
#include "q1filter.h"
#include "q1reader.h"
#include "q1writer.h"
#include "q1hexdumpfilter.h"
#include "q1whitespacefilter.h"
#include "q1trianglefilter.h"

bool convert( int & val, char * buffer ) {		// convert C string to integer
    stringstream ss( buffer );			// connect stream and buffer
    string temp;
    ss >> dec >> val;					// convert integer from buffer
    return ! ss.fail() &&				// conversion successful ?
	! ( ss >> temp );				// characters after conversion all blank ?
} // convert

int main( int argc, char* argv[] ) {
	istream * infile = &cin;	// default value
	ostream * outfile = &cout; 	// default value

	// find positions of filter options in argv
	int filter_positions[argc];
	int filter_count =0;
	int filter_arg_count = 0;
	for ( int i = 0; i < argc; i++ ) {
		char* s = argv[i];
		if ( strcmp(s, "-w") == 0 || strcmp(s, "-h") == 0 || strcmp(s, "-T") == 0) {
			if (strcmp(s, "-T") == 0) filter_arg_count++;
			filter_positions[filter_count] = i;
			filter_count++;
		} // if
	} // for

	// Determine program input and output
	int nonfilter_args = argc - filter_arg_count - filter_count;
	try {
	switch ( nonfilter_args ) {
		case 3: // input and output files are specified
			cout << "both input and out is set" << endl;
			try {
				infile = new ifstream( argv[argc-2] );	// open input file
			} catch( uFile::Failure & ) {
				cerr << "Error! Could not open input file \"" << argv[3] << "\"" << endl;
				throw 1;
			} // try
			try {
				outfile = new ofstream( argv[argc-1] );	// open output file
			} catch( uFile::Failure & ) {		// open failed ?
				cerr << "Error! Could not open output file \"" << argv[4] << "\"" << endl;
				throw 1;
			} // try
			break;
		case 2: // input file only
			try {
				infile = new ifstream( argv[argc-1] );	// open input file
			} catch( uFile::Failure & ) {
				cerr << "Error! Could not open input file \"" << argv[3] << "\"" << endl;
				throw 1;
			} // try
	} // switch
	}  catch (...) {
		cerr << "Usage: " << argv[0]
		<< "[ -filter-options... "
		<< "[ infile [outfile] ]" << endl;
		exit( EXIT_FAILURE );
	} // try

	// Create pipeline
	int filter_pointer = max(0, filter_count-1); // handle no fitler edge case
	int base_arg;
	Filter* next = new Writer(outfile);
	for ( int i = argc - 1; i > 0; i--) {
		int fp = filter_positions[filter_pointer];
		char* flag = argv[fp];
		if ( i == fp ) {
			if ( strcmp(flag, "-w") == 0 ) {
				next = new WhiteSpaceFilter(next);
			}
			else if ( strcmp(flag, "-h") == 0 ){
				next = new HexDumpFilter(next);
			}
			else if ( strcmp(flag, "-T") == 0 ) {
				next = new TriangleFilter(next, base_arg);
			}
			filter_pointer--;
		} // if 
 		// get args for -T options
		else if ( i == fp + 1 && strcmp(flag, "-T") == 0 ) {
			convert(base_arg, argv[i]);
		} // if
	} // for

	// Start pipeline
	Filter * r = new Reader(next, infile);
	delete r;

    if ( infile != &cin ) delete infile;		// close file, do not delete cin!
    if ( outfile != &cout ) delete outfile;		// close file, do not delete cout!
}
// end of file