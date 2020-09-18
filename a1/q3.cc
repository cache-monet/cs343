_Coroutine IntegerLiteral {
    char ch;                            // character passed by cocaller
    // YOU ADD MEMBERS HERE
    void main();                        // coroutine main
  public:
    enum { EOT = '\003' };              // end of text
    _Event Match {                      // last character match
      public:
        unsigned long int value;        // value of integer literal
        Match( unsigned long int value ) : value( value ) {}
    };
    _Event Error {};                    // last character invalid
    void next( char c ) {
        ch = c;                         // communication input
        resume();                       // activate
    }
};

Assignment test data
123
0123
0x123
123u
123L
123ul

07777
9999
0xffffff
07777L
9999U
0xffffffUL

0xeh
1234HIJ
