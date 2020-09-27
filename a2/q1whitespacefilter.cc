/* Implementation for WhitespaceFilter coroutine */
#include "q1filter.h"
#include <ctype.h>
using namespace std;

WhiteSpaceFilter::WhiteSpaceFilter( Filter* next ) {
    this->next = next;
} // WhiteSpaceFilter::WhiteSpaceFilter

void WhiteSpaceFilter::main() {
    try {
        _Enable{
            WSMain: for ( ;; ) {
                TrimLeadingWS: while ( isblank(ch) ) suspend();
                if (ch == '\n') { // ignore blank line
                    suspend();
                    continue WSMain;
                }

                FilterWithinLine: for ( ;; ) {
                    if ( isblank(ch)  ) {
                        TruncateWS: while (isblank(ch)) suspend();
                        if (ch == '\n') { // trailing whitespace
                            next->put(ch);
                            suspend();
                            break FilterWithinLine;
                        }
                        next->put(' '); // truncate 
                    }
                    next->put(ch);
                    suspend();
                }
                // handle whitespace in middle
            }
        } // ENABLE
    } catch (Eof&)  {
        _Resume Eof() _At *next;
        next->put(ch); // put arbitray character
    } // try
} // WhiteSpaceFilter::main
