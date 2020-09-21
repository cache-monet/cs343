#include <iostream>
#include <cstdlib>                                      // access: rand, srand
#include <cstring>                                      // access: strcmp

using namespace std;

enum IntegerType {DECIMAL, OCTAL, HEXADECIMAL, ERROR};
const int MAX_DEC = 19, MAX_OCT = 22, MAX_HEX = 16;
IntegerType tag = IntegerType::DECIMAL;

int main() {
  cin >> noskipws;
  for (;;) {
    string raw; string parsed; string error;
    int val;
    tag = IntegerType::DECIMAL;


    getline(cin, raw);
    if ( cin.fail() ) goto fini;

    if (raw == "\n") {
      cout << "\'\': Warning! Blank line." << endl;
      continue;
    }

    int i = 0;

    for ( char const &rc: raw ) {
      char c = toupper(rc); // converting to upper to check character validity in Hex
      
      if (tag == IntegerType::ERROR) {
          error.push_back(rc);
          continue;
      }

      // First charcter must be between 0 and 9
      if ( i == 0 && !isdigit(c) ) {
          tag = IntegerType::ERROR; parsed.push_back(rc);
          continue;
      }
      if (i == 0 && c == '0') {
        tag = IntegerType::OCTAL;
      }

      // check seconnd character if it is a hex
      if (i == 1 && c == 'X') {
        if (tag == IntegerType::DECIMAL) { // No leading Zero
          tag = IntegerType::ERROR; parsed.push_back(rc);
        }
        tag = IntegerType::HEXADECIMAL;
        parsed.push_back(rc); continue;
      }

      if (c == 'U' || c == 'L' ) continue;

      if ( tag == IntegerType::DECIMAL ) {
        if ( !('0' <= c || c <= '9') ) {
            tag = IntegerType::ERROR;
            parsed.push_back(rc); continue;
        }
        val = val * 10 + c - '0';
      }
      if ( tag == IntegerType::OCTAL ) {
        if ( !('0' <= c || c <= '7') ) {
          tag = IntegerType::ERROR;
          parsed.push_back(rc); continue;
        } 
        val = val * 8 + c - '0';
      }
      if ( tag == IntegerType::HEXADECIMAL ) {
        if (!isxdigit(c)) {
          tag = IntegerType::ERROR;
          parsed.push_back(rc); continue;
        }
        if ( '0' <= c && c <= '9') {
          val = val * 16 + (c - '0');
        }
        else if ('A' <= c && c <= 'F' ) {
          val = val * 16 + (c - 'A' + 10);
        } 
      }
      // check for sign/unsigned
      parsed.push_back(rc); i++;
    }

    cout << '\'' << raw << '\'' << " : ";
  
    if ( tag == IntegerType::ERROR ) {
        cout << "no : " << '\'' << parsed << '\'';
        if (error != "") cout << " - extraneous characters "  << '\'' << error << '\'';
        cout << endl;
    } else {
      cout << "yes, value " << '\'' << val << '\'' << endl;
    } 
  }
fini: ;
return 0;
}
