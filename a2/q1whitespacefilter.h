/* Define interface for WhiteSpaceFilter */
#ifndef _WHITESPACE_FILTER_H
#define _WHITESPACE_FILTER_H

#include "q1filter.h"

_Coroutine  WhiteSpaceFilter : public Filter {
    // YOU MAY ADD PRIVATE MEMBERS
    void main();
  public:
    WhiteSpaceFilter( Filter * f );
};
#endif
// end of file