#include <iostream>
#include <cstdlib>                                      // access: rand, srand
#include <cstring>                                      // access: strcmp
#include <unistd.h>                                     // access: getpid
#include "q3integerliteral.h"
using namespace std;

void IntegerLiteral::parse_hex() {
  if (!isxdigit(ch)) raiseError();
  if ( '0' <= ch && ch <= '9') {
    val = val * 16 + (ch - '0');
  } else if ('A' <= toupper(ch) && toupper(ch) <= 'F' ) {
    val = val * 16 + (toupper(ch) - 'A' + 10);
  } 
}

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
  if ( !isdigit(ch) ) {
    raiseError();
  }

  // Use first character to check integer type
  if (ch == '0') {
    suspend();
    goto CHECK_HEX;
  }

DEC: for (;;) {
  // if (ch == EOT ) cout << "yes " << val;
  if (ch == EOT ) raiseMatch();
  if (toupper(ch) == 'L' || toupper(ch) == 'U') {
    goto CHECK_MATCH;
  }
  if ( !isdigit(ch) ) {
    raiseError();
  }
  val = val * 10 + ch - '0';
  suspend();
} // DEC

CHECK_HEX: {
  if (toupper(ch) == 'X') {
    suspend();
    goto HEX;
  }
  goto OCT; // integer failed Hex check
} // CHECK_HEX

// Integer suffix need to be preceded hexadecimal digits
HEX: {
  parse_hex();  
  suspend();
  for (;;) {
    if (ch == EOT ) raiseMatch();
    if (toupper(ch) == 'L' || toupper(ch) == 'U') goto CHECK_MATCH;
    parse_hex();
    suspend();
  }
} // HEX

OCT: for (;;) {
  if (ch == EOT ) raiseMatch();
  if (toupper(ch) == 'L' || toupper(ch) == 'U') goto CHECK_MATCH;
  if ( ch < '0' || ch > '7' ) raiseError();
  val = val * 8 + ch - '0';
  suspend();
} // OCT

CHECK_MATCH: {
  if (toupper(ch) == 'U') {
    suspend();
    if (toupper(ch) == 'L' || ch == EOT ) raiseMatch();
    raiseError();
  }
  suspend();
  if (toupper(ch) == 'U' || ch == EOT ) raiseMatch();
  Error();
} // CHECK_MATCH

} // coroutine main 

void IntegerLiteral::next( char c ) {
    ch = c;                         // communication input
    resume();                       // activate
} // coroutine next
