//--------------------------------------------------------------------------------------------------------------------
// Define the interface for the Coroutine IntegerLiteral
//--------------------------------------------------------------------------------------------------------------------

_Coroutine IntegerLiteral {
    char ch;                            // character passed by cocaller
    int val = 0;
    void parse_hex();
    void main();
    void raiseError();
    void raiseMatch();
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