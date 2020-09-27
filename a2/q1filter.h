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
#endif
// end of file