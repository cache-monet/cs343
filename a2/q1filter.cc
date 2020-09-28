#include <iostream>
#include "q1filter.h"           // Definition for the Filter interface
using namespace std;

/* FILTER IMPLEMENTATION */
Filter::~Filter() {
    if ( next != NULL ) {
        delete next;
    }
} // Filter::~Filter
void Filter::put(unsigned char c) {
    ch = c;
    resume();
}
