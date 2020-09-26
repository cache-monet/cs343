#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;					// direct access to std
#include <cstdlib>					// exit
// #include "q1filter.h"

bool convert( int & val, char * buffer ) {		// convert C string to integer
    stringstream ss( buffer );			// connect stream and buffer
    string temp;
    ss >> dec >> val;					// convert integer from buffer
    return ! ss.fail() &&				// conversion successful ?
	! ( ss >> temp );				// characters after conversion all blank ?
} // convert

void create_filter( char* option ) {
	if ( strcmp(option, "-w") == 0 ) {
		cout << "whitespace" << endl;
	} else if ( strcmp(option, "-h") == 0 ) {
		cout << "hex dump" << endl;
	} else {
		cout << "invalid input" << endl;
	}
}

int main( int argc, char* argv[] ) {
    istream * infile = &cin; ostream * outfile = &cout; // default values

	// find positions of options
	int filter_positions[argc] = {0};
	int filter_count = 0;
	for ( int i = 0; i < argc; i++ ) {
		char* s = argv[i];
		if ( strcmp(s, "-w") == 0 || strcmp(s, "-h") == 0 || strcmp(s, "-T") == 0) {
			filter_positions[filter_count] = i;
			filter_count++;
		}
	} // for

	// // debug
	// cout << "[";
	// for (auto & f : filter_positions) {
	// 	cout << f << ", ";
	// }
	// cout << "]" << endl;
	
	// get args for any -T
	int filter_pointer = filter_count-1;
	bool input_file = false, output_file = false;
	int arg, arg_count = 0;

	for ( int i = argc - 1; i > 0; i--) {
		int fp = filter_positions[filter_pointer];
		char* flag = argv[fp];
		if ( i == fp ) {
			create_filter( argv[fp] );
			filter_pointer--;
		} else if ( i == fp + 1 && strcmp(flag, "-T") == 0 &&  convert(arg, argv[i]) && arg != -1 ) {
			cout << "Triangle " << arg << endl;
			arg_count++; i++; // increment i to account for -T and its arg
			filter_pointer--;
		} // if
	} // for

	int nonfilter_arg_count = argc - arg_count - filter_count;
	try {
	switch ( nonfilter_arg_count ) {
		case 3: // input and out file are defined
			try {
				infile = new ifstream( argv[argc-2] );	// open input file
			} catch( uFile::Failure & ) {
			cerr << "Error! Could not open input file \"" << argv[3] << "\"" << endl;
			throw 1;
			} // try
			try {
				outfile = new ofstream( argv[argc-1] );	// open output filei
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

    if ( infile != &cin ) delete infile;		// close file, do not delete cin!
    if ( outfile != &cout ) delete outfile;		// close file, do not delete cout!
}
