/* Implementation for Reader coroutine */
#include <iostream>
#include "q1filter.h"
using namespace std;

Reader::Reader (Filter * next, istream * i): in(i) {
    this->next = next;
    resume(); // start coroutine
} // Reader::Reader

void Reader::main() {
    for ( ;; ) {
        *in >> ch;
        if ( in->fail() || in->eof() ) {
            _Resume Eof() _At *next;
            next->put(ch); // pass arbitrary character
            break;
        } // if
        next->put(ch);
    } //for 
} // Reader::main