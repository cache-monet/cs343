#include "q2printer.h"
#include <iostream>

using namespace std;

Printer::Printer(unsigned int voters) : voters(voters) {
#ifndef NOOUTPUT
    cout << "V0";
    for (unsigned int i = 1; i < voters; i++ ) cout << "\tV" << i; // column seperator
    cout << '\n'; // newline
    cout << "*******"; // underline for v0
    for (unsigned int i = 1; i < voters; i++ ) cout << "\t*******";
    cout << '\n';
#endif
    buffer = new VoteState[voters]; // create buffer
} // Printer::Printer


void Printer::print( unsigned int id, Voter::States state ) {
    if (buffer[id].state != VoteState::States::None ) flush();
    buffer[id].state = (VoteState::States) state;
}

void Printer::print( unsigned int id, Voter::States state, TallyVotes::Tour tour ) {
    print(id, state);
    buffer[id].destination = tour.tourkind;
    buffer[id].groupno = tour.groupno;
}

void Printer::print( unsigned int id, Voter::States state, TallyVotes::Ballot vote ) {
    print(id, state);
    buffer[id].ballot = vote;
}

void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked ) {
    print(id, state);
    buffer[id].numBlocked = numBlocked;
}

void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked, unsigned int group ) {
    print(id, state, numBlocked);
    buffer[id].groupno = group;
}

void Printer::flush() {
    for ( unsigned int id = 0; id < voters; id++ ) {
        if (id > 0) cout << "\t";

        VoteState::States state = buffer[id].state;

        if ( state == VoteState::States::None ) continue; // nothing to print continue to next col

        cout << (char) state;
        if ( state == VoteState::States::Block || state == VoteState::States::Unblock ) {
            cout << " " << buffer[id].numBlocked;
        }
        if ( state == VoteState::States::Complete) {
            cout << " " << (char) buffer[id].destination;
        }
        if ( state == VoteState::States::Going) {
            cout << " " << (char) buffer[id].destination << " " << buffer[id].groupno;
        }
        if ( state == VoteState::States::Barging) {
            cout << " " << buffer[id].numBlocked << " " << buffer[id].groupno;
        }
        if ( state == VoteState::States::Vote) {
            cout << " " << buffer[id].ballot.picture <<
            "," << buffer[id].ballot.statue <<
            "," << buffer[id].ballot.giftshop;
        }
        buffer[id].state = VoteState::States::None; // assign current to none
    } // for
    cout << endl;
} // Printer::flush

Printer::~Printer() { // clean up and print end message
#ifndef NOOUTPUT
    flush(); // print anything left in buff
    cout << "*****************" << endl << "All tours started" << endl;
#endif
	delete[] buffer;
} // ~Printer