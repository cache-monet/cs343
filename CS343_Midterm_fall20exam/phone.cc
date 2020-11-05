// COMPILE COMMAND: u++ -g phone.cc -o phone

/* SAMPLE TEST DATA
+1(876)343-8760
(876)343-8760
(800)555-1212
456-9807
786-5555
+1189-6543
789 6543
(888)345-879
-8790
5555
*/

#include <iostream>
#include <fstream>
using namespace std;
#include <ctype.h>                                      // prototype: isdigit

_Coroutine Phone {
    char ch;                // character passed by cocaller
    // YOU ADD MEMBERS HERE
    void main() {
        try {
            for (;;) {
                // area optional
                if (ch == '+') { // if input starts with + it must have an area code
                    suspend();
                    if (ch != '1') _Throw Error();
                    suspend();
                    if (ch != '(') _Throw Error();
                    suspend();
                    goto Area;
                }

                if (ch == '(') { // ok we need area code
                    suspend(); // wait to read in next char
                    goto Area;
                } else {
                    goto Trunk;
                }
                // Area three digits
                Area: {
                    for (int i = 0; i < 3; i++) { // read in 3 digits
                        if (!isdigit(ch)) _Throw Error();
                        suspend();
                    }
                    if (ch != ')') _Throw Error(); // area code must terminate with this )
                    suspend();
                }

                //trunk 3 digits
                Trunk: for (int i = 0; i < 3; i++) {
                    if (!isdigit(ch)) _Throw Error();
                    suspend();
                }

                if (ch != '-') _Throw Error();
                suspend();

                // number 4 digits
                Digit: for (int i = 0; i < 4; i++) {
                    if (!isdigit(ch)) _Throw Error();
                    suspend();
                }

                if (ch == '\n') { // check that stream finished
                    _Resume Match() _At resumer();
                } else { // input is longer than allowed size
                    _Resume Error() _At resumer();
                }
                return;
            } // for
        } catch (Error& ) {
            _Resume Error() _At resumer();
            return;
        } // try
    }; // Phone::main
  public:
    _Event Match {};        // characters form a valid string in the language
    _Event Error {};        // last character results in string not in the language
    void next( char c ) {
        ch = c;
        resume();
    } // Phone::next
}; // Phone

int main() {
    string line;                                        // input phone number

    for ( ;; ) {
        getline( cin, line );                           // read line
      if ( cin.fail() ) break;                          // EOF ?
        cout << "'" << line << "' : '";
        int length = line.length();
        try {
            _Enable {
                Phone phone;
                for ( int i = 0; i < length; ++i ) {
                    cout << line[i];
                    phone.next(line[i]);
                }
                phone.next('\n'); // pass newline to Phone to let signal finished stream
            } // ENABLE
        } catch ( Phone::Match& M ) {
            cout << "' yes" << endl;
        } catch ( Phone::Error& E ) {
            cout << "' no" << endl;
        }// try
    } // for
} // main
