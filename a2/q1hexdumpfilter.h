/* Define interface for HexDumpFilter */
#ifndef _HEX_FILTER_H
#define _HEX_FILTER_H

#include <string>
#include "q1filter.h"

_Coroutine HexDumpFilter: public Filter {
    void main();
    void two_char_to_four_hex();
    std::string to_hex();
  public:
    HexDumpFilter( Filter * f );
};
#endif
// end of file