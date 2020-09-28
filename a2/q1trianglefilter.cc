/* Implementation for TriangleFilter coroutine */
#include "q1filter.h"
#include "q1trianglefilter.h"

TriangleFilter::TriangleFilter( Filter* next, int base ) : height(base / 2) {
    this->next = next;
} //TriangleFilter::TriangleFilter

void TriangleFilter::main() {
    try {
        _Enable {
            TriangleMain: for ( ;; ) {
                TriangleLevels: for (int level = 0; level <= height; level++) 
                {
                    int offset = height - level;
                    Offset: for (int o = 0; o < offset; o++) next->put(' ');
                    Content: for (int c = 0; c < 2 * level + 1; c++) {
                        ch == '\n' ? next->put(' ') : next->put(ch); // replace newline with ' '
                        suspend();
                    }
                    next->put('\n');
                }
            }
        } // ENABLE
    } catch (Eof&)  {
        _Resume Eof() _At *next;
        next->put(ch); // put arbitray character
    } // try
} // TriangleFilter::main
// end of file