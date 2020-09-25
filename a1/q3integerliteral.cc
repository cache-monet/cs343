#include "q3integerliteral.h"

void IntegerLiteral::raiseError() {
  _Resume Error() _At resumer();
  suspend();
}

void IntegerLiteral::raiseMatch() {
  _Resume Match(val) _At resumer();
  suspend();
}

void IntegerLiteral::main() {
  // First character must be a decimal digit
  if ( !isdigit(ch) ) raiseError();

  // If first digit is 0 look at second character to determine if integer is octal or hex
  if (ch == '0') {
    // neither leading 0 nor 0x is counted as digits
    suspend();
    if (toupper(ch) == 'X') {
      tag = IntegerType::HEXADECIMAL;
      suspend();
      // Check for edge cases 0xl 0xu
      // Integer suffix need to be preceded hexadecimal digits
      if (!isxdigit(ch)) raiseError();
    } else {
      tag = IntegerType::OCTAL;
    } // if
  } // if

  for ( ;; ) {
    digits++;
    if (ch == EOT ) raiseMatch();
    if (toupper(ch) == 'L' || toupper(ch) == 'U') tag = IntegerType::SUFFIX; // Begin flow to validate suffix(es)
    switch (tag) {
      case IntegerType::DECIMAL:
        if ( digits > MAX_DIGIT_DEC || !isdigit(ch) ) raiseError();
        val = val * 10 + ch - '0';
        suspend();
        break; 
      case IntegerType::OCTAL:
        if ( digits > MAX_DIGIT_OCT || ch < '0' || ch > '7' ) raiseError();
        val = val * 8 + ch - '0';
        suspend();
        break; 
      case IntegerType::HEXADECIMAL:
        if ( digits > MAX_DIGIT_HEX || !isxdigit(ch) ) raiseError();
        if ( '0' <= ch && ch <= '9') {
          val = val * 16 + (ch - '0');
        } else if ('A' <= toupper(ch) && toupper(ch) <= 'F' ) {
          val = val * 16 + (toupper(ch) - 'A' + 10);
        } // if
        suspend();
        break; 
      case IntegerType::SUFFIX:
        if (toupper(ch) == 'U') {
          suspend();
          if (toupper(ch) == 'L' || ch == EOT ) raiseMatch();
          raiseError();
        }
        suspend();
        if (toupper(ch) == 'U' || ch == EOT ) raiseMatch();
        raiseError();
    } // switch
  } // for
  // free file stream
} // coroutine main 

void IntegerLiteral::next( char c ) {
    ch = c;                         // communication input
    resume();                       // activate
} // coroutine next
// end of file