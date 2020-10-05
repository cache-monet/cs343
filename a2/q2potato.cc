#include <iostream>
using namespace std;
#include "PRNG.h"
#include  "q2potato.h"

extern PRNG prng;

Potato::Potato( unsigned int maxTicks ) {
    reset(maxTicks);
}

void Potato::countdown() {
    timer--;
    if (timer == 0) _Throw Explode();
}

void Potato::reset( unsigned int maxTicks ) {
    timer = prng(1, maxTicks); // choose 
    cout << "  POTATO goes off after " << timer << ( timer == 1 ? " toss" : " tosses") << endl;
}
// end of file