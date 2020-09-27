#ifndef __HEX_FILTER__
#define __HEX_FILTER__

#include <string>
#include "q1filter.h"

_Coroutine HexDumpFilter: public Filter {
    // YOU MAY ADD PRIVATE MEMBERS
    void main();
    void two_char_to_four_hex();
    std::string to_hex();
  public:
    HexDumpFilter( Filter * f );
};
#endif
// end of file