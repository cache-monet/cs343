#ifndef PRINTER_H
#define PRINTER_H

#include "q2voter.h"
#include "q2tallyVotes.h"
 
_Monitor Printer {
  struct VoteState {
    enum States { // basically vote states but with an additional state N representing none
      Start = 'S', Vote = 'V', Block = 'B', Unblock = 'U', Barging = 'b',
      Done = 'D', Complete = 'C', Going = 'G', Failed = 'X', Terminated = 'T', None = 'N'
    }; 

    States state;
    TallyVotes::TourKind destination;
    TallyVotes::Ballot ballot;
    unsigned int numBlocked;
    unsigned int groupno;
    VoteState() { state = None; } // VoteState::VoteState
  };  // VoteState

  unsigned int voters;
  VoteState* buffer; // array to store print states
  void flush(); // prints and clears the buffer

  public:
    Printer( unsigned int voters );
    void print( unsigned int id, Voter::States state );
    void print( unsigned int id, Voter::States state, TallyVotes::Tour tour );
    void print( unsigned int id, Voter::States state, TallyVotes::Ballot vote );
    void print( unsigned int id, Voter::States state, unsigned int numBlocked );
    void print( unsigned int id, Voter::States state, unsigned int numBlocked, unsigned int group );
    ~Printer();
};

#endif