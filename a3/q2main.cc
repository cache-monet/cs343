#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "q2quicksort.h"

// support doubel and int input
typedef TYPE T;

using namespace std;

unsigned int uDefaultStackSize() {
	return 512 * 1000;		// set all task stack-size to 512K
}
// helper function for main to print 
template<typename T>
void print_arr(ostream* o, int length, T* values) {
	if (length == 0) { *o << endl; return; } // edgecase

	const int rlen = 22;
	int rows = length / rlen;
	bool no_remainder = length % rlen == 0; // all ines are exactly 22 characters

	PrintRows: for (int r = 0; r <= rows; r++ ) {
		// *o << "row " << r << ": ";
		PrintRow: for (int i = 0; i < rlen; i++) {
			int index = i + r * rlen ;
			if (index == length ) break PrintRows; // finished printing
			*o << values[index];
			if (index == length - 1 || i == rlen - 1) {
				*o << endl;
			} else {
				*o << " ";
			}
		}
		// doing some boolean algebra
		// don't print "  " after last line
		if (r < rows - (int) no_remainder) *o << "  "; 
	}
}

int main(int argc, char * argv[]) {
	// default values
	bool timed_mode = false; // track timed mode or sort mode
	int depth = 0, size = 0;
	istream * infile = &cin;
	ostream * outfile = &cout;
	try {
		// time mode: depth is required
		if (argc >= 2 && argv[1][0] == '-' && argv[1][1] == 't')  {
			timed_mode = true;
			switch (argc) {
				case 4: depth = stoi( argv[3] ); if ( depth < 0 ) throw 1; 
				case 3: size = stoi( argv[2] ); if ( size < 0 ) throw 1; break;
				default: throw 1;
			}
		} else {
			switch (argc) {
				case 3: if ( strcmp( argv[2], "d" ) != 0 ) {  // default ?
					try {
						outfile = new ofstream( argv[2] );	// open output file
					} catch( uFile::Failure & ) {
						cerr << "Error! Could not open output file \"" << argv[2] << "\"" << endl;
						throw 1;
					} // try
				} // if
				case 2: if ( strcmp( argv[1], "d" ) != 0 ) { // default ?
					try {
						infile = new ifstream( argv[1] );	// open input file
					} catch( uFile::Failure & ) {
						cerr << "Error! Could not open input file \"" << argv[1] << "\"" << endl;
						throw 1;
					} // try
				} // if
			} // switch
		} // else
	} catch (...) {
		cout << "Usage: " << argv[0] << " ( [ unsorted-file | 'd' [ sorted-file | 'd' [ depth (>= 0) ] ] ] | -t size (>= 0) [ depth (>= 0) ] )" << endl;
		exit(1);
	} // try

	uProcessor p[(1<<depth) - 1] __attribute__((unused)); // add processors if that's specified
	if (timed_mode) {
		uProcessor p[(1<<depth) - 1] __attribute__((unused)); // add processors if that's specified
		// create arr of length size and with values between 1->size & and randomize order
		T* values = new T[size];
		for (int i = size; i > 0; i--) values[size-i] = (T) i; // explicitly cast to type T
		unsigned int times = sqrt(size);

		for (unsigned counter = 0; counter < times; counter += 1) swap(values[0], values[rand() % size]); 

		// Benchmark sort time
		uTime start = uClock::currTime();
		quicksort(values, 0, (unsigned int)size - 1, (unsigned int) depth);
		cout << "Sort time "<< uClock::currTime() - start << " sec." << endl;
		goto finished;
	}
	// sort mode
	// read in, sort, and (possibly) print
	for ( ;; ) {
		int length;
		*infile >> length;
    	if ( infile->fail() ) break;

		// allocate memory on heap to store values
		T* values = new T[length];
		for (int i = 0; i < length; i++) *infile >> values[i];
		print_arr(outfile, length, values); // print read array
		if (length != 0) quicksort(values, 0, (unsigned int) length-1, (unsigned int) depth); 
		print_arr(outfile, length, values);
		*outfile << endl; // print separators
		delete values; // cleanup
	}
	finished:
	if (infile != &cin) delete infile;
	if (outfile != &cout) delete outfile;
}