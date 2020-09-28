//--------------------------------------------------------------------------------------------------------------------
// Define the interface for the Coroutine IntegerLiteral
//--------------------------------------------------------------------------------------------------------------------

_Coroutine IntegerLiteral {
    char ch;                            // character passed by cocaller
    enum IntegerType { DECIMAL, OCTAL, HEXADECIMAL, SUFFIX };
    const int MAX_DIGIT_DEC = 20, MAX_DIGIT_OCT = 22, MAX_DIGIT_HEX = 16;
    long unsigned int val = 0; int digits = 0;
    IntegerType tag = IntegerType::DECIMAL;
    void raiseError();
    void raiseMatch();
    void main();
    public:
      enum { EOT = '\003' };              // end of text
      _Event Match {                      // last character match
        public:
          unsigned long int value;        // value of integer literal
          Match( unsigned long int value ) : value( value ) {}
      };
      _Event Error {};                    // last character invalid
      void next( char c );
};