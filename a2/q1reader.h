/* Define interface for ReaderFilter */
#ifndef READER_FILTER_H
#define READER_FILTER_H

#include "q1filter.h"

_Coroutine Reader : public Filter {
    std::istream * in;
    void main();
  public:
    Reader( Filter * f, std::istream * in );
};
#endif
// end of file