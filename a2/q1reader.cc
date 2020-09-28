/* Implementation for Reader coroutine */

#include <iostream>
using namespace std;
#include "q1reader.h"

Reader::Reader (Filter * next, istream * in) {
    this->in = in;
    this->next = next;
    resume(); // start coroutine
} // Reader::Reader

void Reader::main() {
    (*in) >> noskipws;
    for ( ;; ) {
        (*in) >> ch;
        if ( in->fail() || in->eof() ) {
            _Resume Eof() _At *next;
            next->put(ch); // pass arbitrary character
            break;
        } // if
        next->put(ch);
    } //for 
} // Reader::main
// end of file