/* Define interface for abstract Filter class */
#ifndef _FILTER_H
#define _FILTER_H

#include "q1filter.h"

_Coroutine Filter {
  protected:
    _Event Eof {};                    // no more characters
    Filter * next;                    // next filter in chain
    unsigned char ch;                 // communication variable
  public:
      ~Filter();
    void put( unsigned char c );
};
#endif
// end of file