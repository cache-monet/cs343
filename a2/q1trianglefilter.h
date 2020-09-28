/* Define interface for TriangleFilter */
#ifndef _TRIANGLE_FILTER_H
#define _TRIANGLE_FILTER_H

#include "q1filter.h"

_Coroutine TriangleFilter : public Filter {
    int height;
    void main();
  public:
    TriangleFilter( Filter * f, int base );
};
#endif
// end of file