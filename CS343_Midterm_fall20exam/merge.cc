// COMPILE COMMANDS:
// u++ -g merge.cc -DTYPE=int -o merge
// u++ -g merge.cc -DTYPE=double -o merge

/* SAMPLE TEST DATA
8 -5 6 7 8 25 98 100 101
5 -3 6 7 19 99

8 -5.5 6 7.7 8 25.99 98.16 100 101.99
5 -3.4 6 7.7 19 99
*/

#include <iostream>
#include <fstream>
using namespace std;

template<typename T> _Coroutine Merge {
    // YOU ADD MEMBERS HERE
    Merge* partner;
    unsigned int size;
    const T* values;

    T lowest;
    unsigned int nextPosn;

    T* mvalues;
    void main() {
        int i;
        try {
            _Enable {
                for (i = 0; i < size; i++) {
                    if (values[i] > lowest) {
                        partner->mergeTo(values[i], nextPosn); // pass control to partner
                    }
                    mvalues[nextPosn] = values[i];
                    nextPosn++;
                } // for
                _Resume Sentinel() _At *partner; // finished; raise sentinel
                partner->mergeTo(this->lowest, this->nextPosn); // resume partner
                return;
            } // enable
        } _CatchResume(Sentinel&) { // finish copy remaining values
            for (; i < size; i++) {
                mvalues[nextPosn] = values[i];
                nextPosn++;
            } 
        }
    } // Merge::main
  public:
    _Event Sentinel {};                                 // end of merge
    void start( Merge & partner, unsigned int size, const T values[], T mvalues[] ) {
        // init values
        Merge::partner = &partner;
        Merge::size = size;
        Merge::values = values;
        Merge::mvalues = mvalues;
    };

    void mergeTo( T lowest, unsigned int nextPosn ) {
        this->lowest = lowest;
        this->nextPosn = nextPosn;
        resume();
    };
}; // Merge

void input( istream & infile, unsigned int size, TYPE slist[] ) {
    for ( unsigned int i = 0; i < size; i += 1 ) {
        infile >> slist[i];
    } // for
} // input

void output( ostream & outfile, const char * fname, unsigned int size, TYPE slist[] ) {
    outfile << fname << ' ' << size << ": ";
    for ( unsigned int i = 0; i < size; i += 1 ) {
        outfile << slist[i] << ' ';
    } // for
    outfile << endl;
} // output

int main( int argc, char * argv[] ) {
    ifstream infile1( argv[1] );                        // open input files
    ifstream infile2( argv[2] );
    ofstream outfile( argv[3] );                        // open output file

    unsigned int size1, size2;                          // number of values in input files
    infile1 >> size1;
    infile2 >> size2;

    TYPE slist1[size1], slist2[size2];                  // storage for input files
    input( infile1, size1, slist1 );
    output( outfile, "input1", size1, slist1 );

    input( infile2, size2, slist2 );
    output( outfile, "input2", size2, slist2 );

    Merge<TYPE> m1, m2;
    unsigned int msize = size1 + size2;
    TYPE mlist[msize];                                  // storage for merged values
    // YOU INITIALIZE COROUTINES AND RESUME THE APPROPRIATE ONE
    m1.start(m2, size1, slist1, mlist);
    m2.start(m1, size2, slist2, mlist);
    if (slist1[0] < slist2[0]) { // start the tasks with the smallest initial value
        m1.mergeTo(slist2[0], 0);
    } else {
        m2.mergeTo(slist1[0], 0);
    }
    output( outfile, "merged", msize, mlist );
} // main
