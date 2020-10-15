/* Implementation for WhitespaceFilter coroutine */
#include <ctype.h>
#include "q1whitespacefilter.h"

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

                // handle whitespace in middle
                FilterWithinLine: for ( ;; ) {
                    if ( isblank(ch)  ) {
                        TruncateWS: while (isblank(ch)) suspend();
                        if (ch != '\n') next->put(' '); // truncate if not trailing whitespace
                    }
                    next->put(ch);
                    if (ch == '\n') { // line has ended restart from beginning
                        suspend();
                        continue WSMain;
                    }
                    suspend();
                }
            }
        } // ENABLE
    } catch (Eof&)  {
        _Resume Eof() _At *next;
        next->put(ch); // put arbitray character
    } // try
} // WhiteSpaceFilter::main
// end of file