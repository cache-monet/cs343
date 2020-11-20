#include "MPRNG.h"
#include "q2voter.h"
#include "q2printer.h"

extern MPRNG mprng;

Voter::Voter( unsigned int id, unsigned int nvotes, TallyVotes & voteTallier, Printer & printer ) :
    id(id), nvotes(nvotes), voteTallier(voteTallier), printer(printer) {} // Voter::Voter

TallyVotes::Ballot Voter::cast() {                  // cast 3-way vote
    // O(1) random selection of 3 items without replacement using divide and conquer.
    static const unsigned int voting[3][2][2] =  {{ {2,1}, {1,2} }, { {0,2}, {2,0} }, { {0,1}, {1,0} } };
    unsigned int picture = mprng( 2 ), statue = mprng( 1 );
    return (TallyVotes::Ballot){ picture, voting[picture][statue][0], voting[picture][statue][1] };
} // Voter::cast

void Voter::main() {
    yield( mprng( 19 ) ); // yield between 0 - 19 times before starting
    try {
        _Enable {
            for (unsigned int i = 0; i < nvotes; i++) {

                #ifdef NOOUTPUT
                    #define PRINT(...); // print start message
                #else
                    printer.print(id, Start); // print start message
                #endif

                yield( mprng( 4 ) ); // yield between 0 - 4 times 
                TallyVotes::Tour tour = voteTallier.vote(id, cast());
                yield( mprng( 4 ) ); // yield again

                #ifdef NOOUTPUT
                    #define PRINT(...); // going on tour
                #else
                    printer.print(id, Going, tour); // going on tour
                #endif
            } // for
        }  // Enable

    } catch (TallyVotes::Failed& ) {
        #ifdef NOOUTPUT
            #define PRINT(...);
        #else
            printer.print(id, Failed);
        #endif
    } // try

#if defined ( BAR )
    voteTallier.done(id); // print terminate language
#else
    voteTallier.done();
#endif

#ifdef NOOUTPUT
    #define PRINT(...); // print terminated
#else
    printer.print(id, Terminated); // print terminated
#endif

} // Voter::main