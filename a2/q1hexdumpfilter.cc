/* Implementation for HexDumpFilter coroutine */

#include <sstream>
#include <sstream>
#include <string>
using namespace std;
#include <iomanip>
#include "q1hexdumpfilter.h"

#include <iostream>

HexDumpFilter::HexDumpFilter( Filter* next ) {
    this->next = next;
} //HexDumpFilter::HexDumpFilter

string HexDumpFilter::to_hex() {
    stringstream ss;
    ss << setfill('0') << setw(2) << hex << (unsigned int)(ch);
    return ss.str();
}

void HexDumpFilter::two_char_to_four_hex() {
    for (int i = 0; i < 2; i++) {
        string hex = to_hex();
        next->put(hex[0]); next->put(hex[1]);
        suspend();
    }
}

void HexDumpFilter::main() {
    try {
        _Enable{
        HexMain: for ( ;; ) {
                HexFormatLine: for ( int i = 0; i < 4 ; ++i ) {
                    // separate 2 groups of 4 hex characters with 1 space
                    two_char_to_four_hex(); next->put(' '); two_char_to_four_hex(); 
                    // seperate each of those 2 groups with with 3 spaces
                    if (i != 3) { next->put(' '); next->put(' '); next->put(' '); }
                } // HexFormatLine
                next->put('\n'); // full line
            } // HexMain
        } // ENABLE
    } catch (Eof&)  {
        _Resume Eof() _At *next;
        next->put(ch); // put arbitray character
    } // try
} // HexDumpFilter::main
// end of file