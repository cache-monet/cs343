/* Implementation for Writer coroutine */
#include <iostream>
using namespace std;
#include "q1writer.h"

Writer::Writer( ostream * o ): char_count(0) {
    this->out = o;
	this->next = NULL;
}	// Writer::Writer

void Writer::main() {
    try {
        _Enable {
            for (;;) {
                (*out) << ch;
                char_count++;
                suspend();
            }
        } // ENABLE
    } catch (Eof&)  {
        (*out) << char_count << " characters" << endl;
    } // try
} // Writer::main
// end of file