#ifndef __POTATO_H__
#define __POTATO_H__

class Potato {
    // YOU ADD MEMBERS HERE
    unsigned int timer;
  public:
    _Event Explode {};
    Potato( unsigned int maxTicks = 10 );
    void reset( unsigned int maxTicks = 10 );
    void countdown();
};
#endif
// end of file