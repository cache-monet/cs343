/* Define interface for WriterFilter */
#ifndef _WRITER_FILTER_H
#define _WRITER_FILTER_H

#include "q1filter.h"

_Coroutine Writer : public Filter {
    std::ostream * out;
    int char_count;
    void main();
  public:
    Writer( std::ostream * out );
};
#endif
// end of file