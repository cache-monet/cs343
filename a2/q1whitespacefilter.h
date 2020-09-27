#ifndef __WHITESPACE_FILTER__
#define __WHITESPACE_FILTER__
#include "q1filter.h"

_Coroutine  WhiteSpaceFilter : public Filter {
    // YOU MAY ADD PRIVATE MEMBERS
    void main();
  public:
    WhiteSpaceFilter( Filter * f );
};
#endif
// end of file