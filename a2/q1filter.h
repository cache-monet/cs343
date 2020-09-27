//--------------------------------------------------------------------------------------------------------------------
// Define the interface for the Coroutines Filter, Reader, and Writer 
//--------------------------------------------------------------------------------------------------------------------

#ifndef __FILTER__
#define __FILTER__
_Coroutine Filter {
  protected:
    _Event Eof {};                    // no more characters
    Filter * next;                    // next filter in chain
    unsigned char ch;                 // communication variable
  public:
      ~Filter();
    void put( unsigned char c );
};

_Coroutine Reader : public Filter {
    // YOU MAY ADD PRIVATE MEMBERS
    std::istream * in;
    void main();
  public:
    Reader( Filter * f, std::istream * i );
};

_Coroutine Writer : public Filter {
    // YOU MAY ADD PRIVATE MEMBERS
    std::ostream * out;
    int char_count;
    void main();
  public:
    Writer( std::ostream * o );
};

_Coroutine HexDumpFilter: public Filter {
    // YOU MAY ADD PRIVATE MEMBERS
    void main();
  public:
    HexDumpFilter( Filter * f );
};

_Coroutine  WhiteSpaceFilter : public Filter {
    // YOU MAY ADD PRIVATE MEMBERS
    void main();
  public:
    WhiteSpaceFilter( Filter * f );
};

_Coroutine TriangleFilter : public Filter {
    // YOU MAY ADD PRIVATE MEMBERS
    int base;
    void main();
  public:
    TriangleFilter( Filter * f, int base );
};
#endif
// end of file