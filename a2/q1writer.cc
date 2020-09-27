/* Implementation for Writer coroutine */
#include <iostream>
#include "q1filter.h"
using namespace std;

Writer::Writer( ostream * o ): out(o), char_count(0) {
	next = NULL;
}	// Writer::Writer

void Writer::main() {
    try {
        _Enable {
            for (;;) {
                *out << ch;
                char_count++;
                suspend();
            }
        } // ENABLE
    } catch (Eof&)  {
        *out << char_count << " characters" << endl;
    } // try
} // Writer::main
