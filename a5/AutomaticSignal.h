#ifndef AUTOMATIC_SIGNAL_H
#define AUTOMATIC_SIGNAL_H

#define AUTOMATIC_SIGNAL uCondition blocked; 

#define WAITUNTIL(pred, before, after) \
    if (!pred) { \
        before; \
        while( !blocked.empty() )  blocked.signal(); \
        do { \
            blocked.wait(); \
        } while (!pred); \
    } \
    after;

#define EXIT() \
    while ( !blocked.empty() ) blocked.signal();

#endif